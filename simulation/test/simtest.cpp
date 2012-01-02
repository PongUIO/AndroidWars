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
		
		void shoot() { printf("Bang!\n"); }
};

// Starts the simulation
void startSim()
{
	Sim::Configuration config;
	config.phaseLength = 25;
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

void loadSides()
{
	Sim::Player testSide;
	testSide.allyGroup = 0;
	
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
	botCfg.mWeaponBox.add( weapon->getId() );
	uint32_t botId = sim.getState().getBotFactory().createBot( botCfg );
	
	// Give the bot some input
	using namespace Sim::Prog;
	Sim::InputManager &inMgr = sim.getState().getInputManager();
	Sim::ProgramFactory &progFact = sim.getState().getProgramFactory();
	
	MoveTowards *move = progFact.createProgram<MoveTowards>(
		MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(5,5)));
	
	Kill *kill = progFact.createProgram<Kill>(Kill::Config(move->getId()));
	
	inMgr.registerInput(botId, move->getId(), 0);
	inMgr.registerInput(botId, kill->getId(), 5);
	
	sim.getState().getWorld().getTile(3,0).setType(1);
}


int main(void)
{
	startSim();
	loadData();
	setupWorld();
	
	// Run a test phase
	sim.prepareSim();
	for(int i = 0; i < 3; i++) {
		sim.startPhase();
		while( sim.hasPhaseStep() ) {
			Sim::Vector pos = sim.getState().getBotFactory().
				getBot(0)->getBody().mPos;
			printf("%02d (%g, %g)\n", sim.getCurPhase(), pos.x, pos.y);
			
			sim.step();
		}
		
		if(i==1) {
			sim.endPhase();
			
			// Example of giving input after a phase
			using namespace Sim::Prog;
			Sim::InputManager &inMgr = sim.getState().getInputManager();
			Sim::ProgramFactory &progFact = sim.getState().getProgramFactory();
			
			MoveTowards *move = progFact.createProgram<MoveTowards>(
				MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(-50,0))
			);
			
			Kill *kill = progFact.createProgram<Kill>(
				Kill::Config(move->getId()));
			
			inMgr.registerInput(0, move->getId(), 3);
			inMgr.registerInput(0, kill->getId(), 10);
		} else
			sim.rewindPhase();
	}
	
	printf("%X == %X\n", sim.checksumSim(), sim.save().checksum());
	
	printf("Savefile size: %g KiB\n", double(sim.save().size())/1024.0);
	FILE *fp = fopen("save", "w");
	fwrite(sim.save().getData(), sim.save().size(), 1, fp);
	fclose(fp);
	
	// Shutdown the simulation
	sim.shutdown();
	
	return 0;
}
