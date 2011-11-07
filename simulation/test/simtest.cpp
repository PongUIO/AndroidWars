#include <stdio.h>
#include "../Simulation.h"

// Main simulation object
Sim::Simulation sim;

// Starts the simulation
void startSim()
{
	Sim::Configuration config;
	config.phaseLength = 25;
	config.stepTime = 0.01;
	
	sim.startup(config);
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
	
	sim.getData().getBotDb().addBot(botType, pts);
}

void loadWeapons()
{
	Sim::BulletD *bulletType = sim.getData().getBulletDb().createType();
	
	Sim::StateSys *sys = bulletType->getStateSys();
	sys->registerState(new Sim::StdState::Delay(0.2));
	
	class DemoState : public Sim::StateSys::State {
		public:
			void exec(Sim::StateSys::Reference::Thread &t) const {
				printf("%p: DemoState called\n", this);
				t.mActive = nextState();
			}
	};
	sys->registerState(new DemoState());
	sys->registerEntryPoint(0);
	
	Sim::WeaponD *weaponType = sim.getData().getWeaponDb().createType();
	
	Sim::StateSys *wsys = weaponType->getStateSys();
	wsys->registerState(new Sim::WeaponState::Shoot( bulletType->getId() ) );
	wsys->registerEntryPoint(0);
}

// "Loads" simulation data
void loadData()
{
	loadSides();
	loadTiles();
	loadBots();
	loadWeapons();
}

void setupWorld()
{
	// Create a test bot
	Sim::Bot::Config botCfg;
	Sim::Weapon::Config weapCfg;
	botCfg.side = 0;
	botCfg.type = 0;
	botCfg.body.mPos = Sim::Vector(0,0);
	
	weapCfg.type = 0;
	botCfg.weaponBox.add( sim.getState().getWeaponFactory().create(weapCfg) );
	uint32_t botId = sim.getState().getBotFactory().createBot( botCfg );
	
	// Send some input to this bot
	Sim::BotFactory &botFact = sim.getState().getBotFactory();
	Sim::BotInput bi;
	
	bi = Sim::BotInput::inMove(botId, 20, Sim::Vector(1,0) );
	botFact.getInput().addInput( bi );
	
	bi = Sim::BotInput::inMove(botId, 20, Sim::Vector(-1,0) );
	botFact.getInput().addInput( bi );
	
	/*bi = Sim::BotInput::inShoot(botId, 0, Sim::Vector(1,0) );
	botFact.getInput().addInput( bi );*/
	
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
			
			const Sim::Bullet *bu = sim.getState().getBulletFactory().getObj(0);
			if(bu) {
				pos = bu->getBody().mPos;
				printf("%02d (%g, %g, bullet)\n", sim.getCurPhase(), pos.x,pos.y);
			}
			
			sim.step();
		}
		
		if(i==1)
			sim.endPhase();
		else
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
