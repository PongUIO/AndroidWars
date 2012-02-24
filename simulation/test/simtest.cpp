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
				i!=atmVec.end(); ++i) {
				printHull(*i);
			}
		}
		
		void shoot(Sim::Bot *bot, Sim::Save &arg) {
			/*printf("Before:\n");
			printBotArmor(bot);*/
			bot->getHealth().causeDamage(mSim, 40, 1);
			/*printf("After:\n");
			printBotArmor(bot);
			printf("\n");*/
		}
};

// Starts the simulation
void startSim()
{
	Sim::Configuration &config = sim.getConfiguration();
	config.phaseLength = 5;
	config.stepTime = 0.01;
	
	sim.startup();
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
	
	{
		using namespace Sim::Abil;
		
		Sim::IdType abilId = sim.getInput().getAbilityInput().
			buildInputImpl<CpuBoost>(CpuBoost::Config(0,10))->getId();
		botCfg.mAbility.addAbility(abilId);
	}
	
	{
		Sim::IdType weapId = sim.getInput().getWeaponInput().
			buildInputImpl<DemoWeapon>(weapCfg)->getId();
		botCfg.mWeapon.addWeapon( weapId );
	}
	
	Sim::IdType botId = sim.getInput().getBotInput().buildInput( botCfg )->getId();
	
	// Create a second dummy bot
	botCfg = Sim::Bot::Config();
	botCfg.mSide = 0;
	botCfg.mType = 0;
	botCfg.mBody.mPos = Sim::Vector(50,50);
	
	sim.getInput().getBotInput().buildInput( botCfg );
	
	// Give the bot some input
	{
		using namespace Sim::Prog;
		
		Sim::FactoryInput<Sim::ProgramFactory> &progInput =
			sim.getInput().getProgramInput();
		Sim::BotCpuInput &cpuInput =
			sim.getInput().getCpuInput();
		
		
		Sim::IdType moveId = progInput.buildInputImpl<MoveTowards>(
			MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(10,0)))->
			getId();
		
		Sim::IdType killId = progInput.buildInputImpl<Kill>(
			Kill::Config(moveId))->getId();
		
		Shoot *shoot = progInput.buildInputImpl<Shoot>(
			Shoot::Config(0, Sim::Save()));
		shoot->setRunningTime(2);
		
		cpuInput.registerInput(botId, moveId, 0);
		cpuInput.registerInput(botId, killId, 3);
		cpuInput.registerInput(botId, shoot->getId(), 5);
	}
	
	sim.getState().getWorld().setDimensions(32,32);
	sim.getState().getWorld().getTile(3,5).setType(1);
	
	// Create abilities and distribute them
	{
		using namespace Sim::Abil;
		
		Sim::FactoryInput<Sim::AbilityFactory> &abilIn =
			sim.getInput().getAbilityInput();
		
		CpuBoost *sharedBoost = abilIn.buildInputImpl<CpuBoost>(
			CpuBoost::Config(0, 5));
		ArmorAttachment *armorAtm =
			abilIn.buildInputImpl<ArmorAttachment>(
			ArmorAttachment::Config(2, 10));
		
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
	
	// Simulate a few finalized phases
	sim.prepareSim();
#define PHASE_COUNT 8
	
	for(int i = 0; i < PHASE_COUNT; i++) {
		sim.startPhase();
		
		while( sim.hasPhaseStep() ) {
			Sim::Bot *bot = sim.getState().getBotFactory().getBot(0);
			Sim::Vector pos;
			if(bot)
				pos = bot->getBody().mPos;
			
			printf("%02d %03d (%g, %g)\n",
				sim.getCurPhase(), sim.getCurPhaseStep(),
				pos.x, pos.y);
			
			sim.step();
		}
		
		sim.endPhase(true);
		printf("Phase %02d checksum: %X\n", sim.getCurPhase(), sim.checksumSim());
		
		if(i==1) {
			// Example of giving input after a phase
			using namespace Sim::Prog;
			Sim::FactoryInput<Sim::ProgramFactory> &progIn =
				sim.getInput().getProgramInput();
			Sim::BotCpuInput &cpuIn =
				sim.getInput().getCpuInput();
			
			MoveTowards *move = progIn.buildInputImpl<MoveTowards>(
				MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(-50,0))
			);
			
			Kill *kill = progIn.buildInputImpl<Kill>(
				Kill::Config(move->getId()));
			
			cpuIn.registerInput(0, move->getId(), 2);
			cpuIn.registerInput(0, kill->getId(), 8);
		} else if(i==3) {
			// Example of creating a new bot in the middle of a phase
			Sim::FactoryInput<Sim::BotFactory> &botIn =
				sim.getInput().getBotInput();
			
			Sim::Bot::Config botCfg;
			botCfg.mSide = 0;
			botCfg.mType = 0;
			botCfg.mBody.mPos = Sim::Vector(5,25);
				
			botIn.buildInput( botCfg );
		}
	}
	
	printf("\n\n");
	
	// Re-run the simulation to assure it is deterministic
	printf("Simulation replay:\n");
	sim.getReplayManager().rewind(0,0);
	bool doLoadInput = false;
	for(int i=0; i<PHASE_COUNT; i++) {
		if(doLoadInput)
			sim.getReplayManager().loadCurPhaseInput();
		else
			doLoadInput=true;
		
		sim.startPhase();
		
		while( sim.hasPhaseStep() ) {
			Sim::Bot *bot = sim.getState().getBotFactory().getBot(0);
			Sim::Vector pos;
			if(bot)
				pos = bot->getBody().mPos;
			
			printf("%02d %03d (%g, %g)\n",
				sim.getCurPhase(), sim.getCurPhaseStep(),
				pos.x, pos.y);
			
			sim.step();
		}
		
		sim.endPhase(false);
		printf("Phase %02d checksum: %X\n", sim.getCurPhase(), sim.checksumSim());
	}
	
	printf("\n\nPresent checksum vs. rewind checksum\n");
	sim.gotoPresent();
	uint32_t presentChecksum = sim.checksumSim();
	sim.getReplayManager().rewind(sim.getCurPhase(),sim.getCurPhaseStep());
	uint32_t rewindChecksum = sim.checksumSim();
	
	printf("%X == %X\n", presentChecksum, rewindChecksum);
	
	printf("Testing simulation saving:\n");
	for(int i=0; i<2; i++) {
		if(i==0)
			sim.gotoPresent();
		else
			sim.getReplayManager().rewind(sim.getCurPhase(),sim.getCurPhaseStep());
		
		printf("Savefile size: %g KiB\n", double(sim.save().size())/1024.0);
		FILE *fp = fopen((i==0) ? "presentSave" : "rewindSave", "w");
		fwrite(sim.save().getData(), sim.save().size(), 1, fp);
		fclose(fp);
	}
	
	// Shutdown the simulation
	sim.shutdown();
	
	return 0;
}
