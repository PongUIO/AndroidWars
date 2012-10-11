#include <stdio.h>
#include "../Simulation.h"

// Main simulation object
Sim::Simulation sim;

// Basic ID allocator
Sim::IdType newId()
{
	static Sim::IdType curId = 0;
	
	return curId++;
}

Sim::IdType BotIdA = Sim::NoId;

/**
 * Simple test implementation of a weapon
 */
class DemoWeapon : public Sim::Weapon {
	public:
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
	Sim::ProgramDatabase &db = sim.getData().getProgramDb();
	
	using namespace Sim::Prog;
	
	db.registerImpl<MoveTowards>("MoveTowards");
	db.registerImpl<Kill>("Kill");
	db.registerImpl<Shoot>("Shoot");
}

void loadAbilities()
{
	Sim::AbilityDatabase &db = sim.getData().getAbilityDb();
	
	using namespace Sim::Abil;
	
	db.registerImpl<ArmorAttachment>("ArmorAttachment");
	db.registerImpl<CpuBoost>("CpuBoost");
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
	
	botType.setCollision(new Sim::Collision(pts));
	
	botType.baseSpeed = 1.0;
	botType.baseWeight = 75.0;
	
	botType.cpuCycleSpeed = 5;
	botType.cpuStorage = 20;
	
	botType.coreHealth.getCore() = Sim::Health::Hull(0, 50);
	botType.coreHealth.addAttachment( Sim::Health::Hull(1, 5) );
	
	sim.getData().getBotDb().
		registerImpl<Sim::Bot>("Base",botType);
}

void loadWeapons()
{
	sim.getData().getWeaponDb().registerImpl<DemoWeapon>("DemoWeapon");
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
	botCfg.mBody.mPos = Sim::Vector(0,0);
	
	{
		using namespace Sim::Abil;
		
		Sim::IdType abilId = sim.getState().getAbilityFactory().
			createImpl<CpuBoost>(
				CpuBoost::Config(0,10),
				sim.getData().getAbilityDb().getIdOf("CpuBoost"),
				newId()
			)->getId();
		botCfg.mAbility.addAbility(abilId);
	}
	
	{
		Sim::IdType weapId = sim.getState().getWeaponFactory().
			createImpl<DemoWeapon>(
				weapCfg,
				sim.getData().getWeaponDb().getIdOf("DemoWeapon"),
				newId()
  			)->getId();
		botCfg.mWeapon.addWeapon( weapId );
	}
	
	BotIdA = sim.getState().getBotFactory().createImpl<Sim::Bot>(
		botCfg, 0, newId())->getId();
	
	// Create a second dummy bot
	botCfg = Sim::Bot::Config();
	botCfg.mSide = 0;
	botCfg.mBody.mPos = Sim::Vector(50,50);
	
	sim.getState().getBotFactory().createImpl<Sim::Bot>(botCfg, 0, newId());
	
	// Give the bot some input
	{
		using namespace Sim::Prog;
		
		Sim::ProgramFactory &progFact =
			sim.getState().getProgramFactory();
		Sim::BotFactory &botFact =
			sim.getState().getBotFactory();
		
		Sim::IdType moveId = progFact.createImpl<MoveTowards>(
			MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(10,0)),
			sim.getData().getProgramDb().getIdOf("MoveTowards"), newId())->getId();
		
		Sim::IdType killId = progFact.createImpl<Kill>(
			Kill::Config(moveId),
			sim.getData().getProgramDb().getIdOf("Kill"), newId())->getId();
		
		Shoot *shoot = progFact.createImpl<Shoot>(
			Shoot::Config(0, Sim::Save()), 
			sim.getData().getProgramDb().getIdOf("Shoot"), newId());
		shoot->setRunningTime(2);
		
		botFact.getBot(BotIdA)->getState().mCpu.scheduleProgram(moveId, 0);
		botFact.getBot(BotIdA)->getState().mCpu.scheduleProgram(killId, 3);
		botFact.getBot(BotIdA)->getState().mCpu.scheduleProgram(shoot->getId(), 5);
	}
	
	sim.getState().getWorld().setDimensions(32,32);
	sim.getState().getWorld().getTile(3,5).setType(1);
	
	// Create abilities and distribute them
	{
		using namespace Sim::Abil;
		
		Sim::AbilityFactory &abilFact =
			sim.getState().getAbilityFactory();
		
		CpuBoost *sharedBoost = abilFact.createImpl<CpuBoost>(
			CpuBoost::Config(0, 5),
			sim.getData().getAbilityDb().getIdOf("CpuBoost"), newId());
		ArmorAttachment *armorAtm =
			abilFact.createImpl<ArmorAttachment>(
			ArmorAttachment::Config(2, 10),
			sim.getData().getAbilityDb().getIdOf("ArmorAttachment"), newId());
		
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
		
		sim.endPhase();
		printf("Phase %02d checksum: %X\n", sim.getCurPhase(), sim.checksumSim());
		
		using namespace Sim::Prog;
		Sim::ProgramFactory &progFact =
			sim.getState().getProgramFactory();
		Sim::BotFactory &botFact =
			sim.getState().getBotFactory();
		if(i==1) {
			// Example of giving input after a phase
			
			MoveTowards *move = progFact.createImpl<MoveTowards>(
				MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(-50,0)),
				sim.getData().getProgramDb().getIdOf("MoveTowards"),
				newId()
			);
			
			Kill *kill = progFact.createImpl<Kill>(
				Kill::Config(move->getId()),
				sim.getData().getProgramDb().getIdOf("Kill"),
				newId()
			);
			
			botFact.getBot(BotIdA)->getState().mCpu.scheduleProgram(move->getId(), 2);
			botFact.getBot(BotIdA)->getState().mCpu.scheduleProgram(kill->getId(), 8);
		} else if(i==3) {
			// Example of creating a new bot in the middle of a phase
			Sim::Bot::Config botCfg;
			botCfg.mSide = 0;
			botCfg.mBody.mPos = Sim::Vector(5,25);
				
			botFact.createImpl<Sim::Bot>( botCfg, 0, newId());
		}
	}
	
	printf("\n\n");
		
	printf("Testing simulation saving:\n");
	FILE *fp = fopen("save", "w");
	Sim::Save save = sim.save();
	fwrite(save.getData(), sim.save().size(), 1, fp);
	fclose(fp);
	printf("Savefile size: %g KiB\n", double(save.size())/1024.0);
	
	// Shutdown the simulation
	sim.shutdown();
	
	return 0;
}
