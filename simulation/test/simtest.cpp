#include "../Simulation.h"

int main(void)
{
	Sim::Simulation sim;
	
	Sim::Configuration config;
	config.phaseLength = 25;
	config.stepTime = 0.01;
	
	sim.startup(config);
	
	uint32_t id = sim.getBotFactory().createBot(0);
	sim.getBotInput().addInput( Sim::BotInput(id, 25) );
	sim.getBotInput().addInput( Sim::BotInput(id, 75) );
	
	sim.startPhase();
	
	while( sim.hasPhaseStep() )
		sim.step();
	
	sim.endPhase();
	
	sim.shutdown();
	
	return 0;
}
