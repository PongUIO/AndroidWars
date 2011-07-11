#include <stdio.h>
#include "../Simulation.h"

int main(void)
{
	Sim::Simulation sim;
	
	// Start the simulation
	Sim::Configuration config;
	config.phaseLength = 25;
	config.stepTime = 0.01;
	
	sim.startup(config);
	
	// Create a test side
	Sim::Player testSide;
	testSide.allyGroup = 0;
	
	sim.getState().getPlayerData().addPlayer(testSide);
	
	// Create test tile types
	Sim::TileD tileType;
	tileType.colMask = Sim::TileD::ColNone;
	
	sim.getData().getTileDb().addTile(tileType);
	
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
	
	// Create a test bot
	Sim::Bot::Config botCfg;
	botCfg.side = 0;
	botCfg.type = 0;
	botCfg.pos = Sim::Vector(0,0);
	uint32_t botId = sim.getState().getBotFactory().createBot( botCfg );
	
	// Send some input to this bot
	Sim::BotFactory &botFact = sim.getState().getBotFactory();
	Sim::BotInput bi;
	
	bi = Sim::BotInput::inMove(botId, 20, Sim::Vector(1,0) );
	botFact.getInput().addInput( bi );
	
	bi = Sim::BotInput::inMove(botId, 20, Sim::Vector(-1,0) );
	botFact.getInput().addInput( bi );
	
	// Run a test phase
	sim.prepareSim();
	for(int i = 0; i < 3; i++) {
		sim.startPhase();
		while( sim.hasPhaseStep() ) {
			const Sim::Vector &pos =
				botFact.getBot(botId)->getBody().mPos;
			printf("%02d (%g, %g)\n", sim.getCurPhase(), pos.x, pos.y);
			sim.step();
		}
		
		if(i==1)
			sim.endPhase();
		else
			sim.rewindPhase();
	}
	
	printf("%X == %X\n", sim.checksumSim(), sim.save().checksum());
	
	printf("Savefile size: %g KiB\n", double(sim.save().size())/1024.0);
	
	// Shutdown the simulation
	sim.shutdown();
	
	return 0;
}
