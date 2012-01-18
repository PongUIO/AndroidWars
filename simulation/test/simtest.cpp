#include <stdio.h>
#include "../Simulation.h"

// Main simulation object
Sim::Simulation sim;

/**
 * Simple test implementation of a weapon
 */
class DemoWeapon : public Sim::Weapon {
	public:
		SIM_WEAPON_HEADER("Test/DemoWeapon")
		
		struct Config {
			Config() {}
		};
		
		DemoWeapon(Sim::Simulation* sim, uint32_t id, uint32_t typeId,
			const Config &cfg) : Weapon(sim, id, typeId) {}
		~DemoWeapon() {}
		
	protected:
		void save(Sim::Save::BasePtr& fp) {}
		void load(Sim::Save::BasePtr& fp) {}
		
		void step(double stepTime) {}
		
		void printHull(Sim::Health::Hull &hull) {
			printf("\t\t%-10s : %3d / %3d\n",
				sim.getData().getArmorDb().getNameOf(hull.getType()).c_str(),
				hull.getHealth(), hull.getMaxHealth() );
		}
		
		void printBotArmor(Sim::Bot *bot) {
			printf("\tCore:\n");
			printHull(bot->getState().mHealth.getCore());
			
			printf("\tAttachments:\n");
			Sim::Health::AttachmentVec  atmVec =
				bot->getState().mHealth.getAttachments();
			for(Sim::Health::AttachmentVec::iterator i=atmVec.begin();
				i!=atmVec.end(); i++) {
				printHull(*i);
			}
		}
		
		void shoot(Sim::Bot *bot, Sim::Save &arg) {
			printf("Before:\n");
			printBotArmor(bot);
			bot->getHealth().causeDamage(mSim, 20, 1);
			printf("After:\n");
			printBotArmor(bot);
			printf("\n");
		}
};

// Starts the simulation
void startSim()
{
	Sim::Configuration config;
	config.phaseLength = 5;
	config.stepTime = 0.01;
	
	sim.startup(config);
}

void loadPrograms()
{
	sim.getData().getProgramDb().registerAllDefault();
}

void loadAbilities()
{
	sim.getData().getAbilityDb().registerAllDefault();
}

void loadDamageArmor()
{
	Sim::ArmorDatabase &armorDb = sim.getData().getArmorDb();
	Sim::DamageDatabase &damageDb = sim.getData().getDamageDb();
	
	damageDb.newDamage("Normal");
	damageDb.newDamage("AntiLight");
	
	Sim::ArmorD *armor = armorDb.newArmor("Light");
	Sim::ArmorD *shield = armorDb.newArmor("Shield");
	Sim::ArmorD *hardened = armorDb.newArmor("Hardened");
	
	armor->registerRule(1, Sim::ArmorD::DamageRule(2.0) );
	shield->registerRule(1, Sim::ArmorD::DamageRule(0.3) );
	hardened->setDefaultRule( Sim::ArmorD::DamageRule(0.1) );
}

void loadSides()
{
	Sim::Player testSide;
	testSide.mAllyGroup = 0;
	
	sim.getState().getPlayerData().addPlayer(testSide);
}

void loadTiles()
{
	Sim::TileD tileType;
	tileType.colMask = Sim::TileD::ColNone;
	
	sim.getData().getTileDb().addTile(tileType);
	
	tileType.colMask = Sim::TileD::ColAll;
	sim.getData().getTileDb().addTile(tileType);
}

void loadBots()
{
	// Create a test bot type
	Sim::BotD botType;
	Sim::Collision::ColPoints pts;
	pts.push_back(Sim::Vector(0,0));
	pts.push_back(Sim::Vector(0,1));
	pts.push_back(Sim::Vector(1,1));
	pts.push_back(Sim::Vector(1,0));
	
	botType.baseSpeed = 1.0;
	botType.baseWeight = 75.0;
	
	botType.cpuCycleSpeed = 5;
	botType.cpuStorage = 20;
	
	botType.coreHealth.getCore() = Sim::Health::Hull(0, 50);
	botType.coreHealth.addAttachment( Sim::Health::Hull(1, 5) );
	
	sim.getData().getBotDb().addBot(botType, pts);
}

void loadWeapons()
{
	sim.getData().getWeaponDb().registerType<DemoWeapon>();
}

// "Loads" simulation data
void loadData()
{
	loadPrograms();
	loadAbilities();
	loadDamageArmor();
	loadSides();
	loadTiles();
	loadBots();
	loadWeapons();
}

void setupWorld()
{
	// Create a test bot
	Sim::Bot::Config botCfg;
	DemoWeapon::Config weapCfg;
	botCfg.mSide = 0;
	botCfg.mType = 0;
	botCfg.mBody.mPos = Sim::Vector(0,0);
	
	using namespace Sim::Abil;
	uint32_t abilId = sim.getState().getAbilityFactory()
		.createAbility<CpuBoost>(CpuBoost::Config(0, 10))->getId();
	botCfg.mAbility.addAbility(abilId);
	
	Sim::Weapon *weapon = sim.getState().getWeaponFactory().createType<DemoWeapon>(weapCfg);
	botCfg.mWeapon.addWeapon( weapon->getId() );
	uint32_t botId = sim.getState().getBotFactory().createBot( botCfg );
	
	// Create a second dummy bot
	botCfg = Sim::Bot::Config();
	botCfg.mSide = 0;
	botCfg.mType = 0;
	botCfg.mBody.mPos = Sim::Vector(50,50);
	
	sim.getState().getBotFactory().createBot( botCfg );
	
	// Give the bot some input
	using namespace Sim::Prog;
	Sim::InputManager &inMgr = sim.getState().getInputManager();
	Sim::ProgramFactory &progFact = sim.getState().getProgramFactory();
	
	MoveTowards *move = progFact.createProgram<MoveTowards>(
		MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(10,0)));
	
	Kill *kill = progFact.createProgram<Kill>(Kill::Config(move->getId()));
	
	Shoot *shoot = progFact.createProgram<Shoot>(Shoot::Config(0, Sim::Save()));
	shoot->setRunningTime(2);
	
	inMgr.registerInput(botId, move->getId(), 0);
	inMgr.registerInput(botId, kill->getId(), 3);
	inMgr.registerInput(botId, shoot->getId(), 5);
	
	sim.getState().getWorld().getTile(3,5).setType(1);
	
	// Create abilities and distribute them
	{
		using namespace Sim::Abil;
		
		Sim::AbilityFactory &abilFact = sim.getState().getAbilityFactory();
		
		CpuBoost *sharedBoost =
			abilFact.createAbility<CpuBoost>( CpuBoost::Config(0, 5) );
		ArmorAttachment *armorAtm =
			abilFact.createAbility<ArmorAttachment>(
			ArmorAttachment::Config(2, 10)
		);
		
		Sim::Player &player = sim.getState().getPlayerData().getPlayer(0);
		player.mGlobalAbilities.push_back(sharedBoost->getId());
		player.mGlobalAbilities.push_back(armorAtm->getId());
		
		armorAtm->setDuration(6);
	}
}


int main(void)
{
	startSim();
	loadData();
	setupWorld();
	
	// Run a test phase
	sim.prepareSim();
	for(int i = 0; i < 4; i++) {
		sim.startPhase();
		
		while( sim.hasPhaseStep() ) {
			Sim::Vector pos = sim.getState().getBotFactory().
				getBot(0)->getBody().mPos;
			printf("%02d %03d (%g, %g)\n",
				sim.getCurPhase(), sim.getCurPhaseStep(),
				pos.x, pos.y);
			
			sim.step();
		}
		
		// For iteration 0, only run a non-final 
		if(i == 1) {
			sim.endPhase(false);
			
			// This loads the 'present' time, or what is internally
			// considered the real simulation state by the replay manager
			sim.getReplayManager().gotoPresent();
		} else {
			sim.endPhase(true);
		}
		
		if(i==1) {
			// Example of giving input after a phase
			using namespace Sim::Prog;
			Sim::InputManager &inMgr = sim.getState().getInputManager();
			Sim::ProgramFactory &progFact = sim.getState().getProgramFactory();
			
			MoveTowards *move = progFact.createProgram<MoveTowards>(
				MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(-50,0))
			);
			
			Kill *kill = progFact.createProgram<Kill>(
				Kill::Config(move->getId()));
			
			inMgr.registerInput(0, move->getId(), 2);
			inMgr.registerInput(0, kill->getId(), 8);
		}
	}
	
	printf("%X == %X\n", sim.checksumSim(), sim.save().checksum());
	
	printf("Savefile size: %g KiB\n", double(sim.save().size())/1024.0);
	FILE *fp = fopen("save", "w");
	fwrite(sim.save().getData(), sim.save().size(), 1, fp);
	fclose(fp);
	
	// Test rewinding
#define REWIND_COUNT 5
	double timeUnit[REWIND_COUNT] = {0.26, 0.2, 0.1, 0.4, 300.0};
	
	for(uint32_t i=0; i<REWIND_COUNT; i++) {
		printf("Rewinding the simulation to timeunit %g\n", timeUnit[i]);
		
		sim.getReplayManager().rewind(timeUnit[i]);
		Sim::Vector pos = sim.getState().getBotFactory().
			getBot(0)->getBody().mPos;
			
		printf("Bot had position (%g, %g)\n", pos.x, pos.y);
		printf("\tat phase %d, step %d\n", sim.getCurPhase(),
			sim.getCurPhaseStep());
	}
	
	// Shutdown the simulation
	sim.shutdown();
	
	return 0;
}
