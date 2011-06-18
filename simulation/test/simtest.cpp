#include "../Simulation.h"

int main(void)
{
	Sim::Simulation sim;
	
	// Start the simulation
	Sim::Configuration config;
	config.phaseLength = 25;
	config.stepTime = 0.01;
	
	sim.startup(config);
	
	// Create a test bot
	Sim::Bot::Config botCfg;
	botCfg.side = 0;
	botCfg.pos = Sim::Vector(0,0);
	uint32_t id = sim.getBotFactory().createBot( botCfg );
	
	// Send some input to this bot
	sim.getBotInput().addInput( Sim::BotInput(id, 25) );
	sim.getBotInput().addInput( Sim::BotInput(id, 75) );
	
	// Run a test phase
	sim.startPhase();
	while( sim.hasPhaseStep() )
		sim.step();
	sim.endPhase();
	
	// Shutdown the simulation
	sim.shutdown();
	
	return 0;
}
