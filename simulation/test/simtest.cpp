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
	Sim::Side testSide;
	testSide.allyGroup = 0;
	
	sim.getState().getSideData().addSide(testSide);
	
	// Create a test bot
	Sim::Bot::Config botCfg;
	botCfg.side = 0;
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
			printf("(%g, %g)\n", pos.x, pos.y);
			sim.step();
		}
		
		sim.endPhase();
		
		if(i==1)
			sim.finalizePhase();
		else
			sim.rewindPhase();
	}
		
	// Shutdown the simulation
	sim.shutdown();
	
	return 0;
}
