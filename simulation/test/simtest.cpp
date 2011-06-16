#include "../Simulation.h"

int main(void)
{
	Sim::Simulation sim;
	
	Sim::Configuration config;
	config.phaseTime = 10.0;
	config.stepTime = 0.01;
	
	sim.startup(config);
	
	uint32_t id = sim.getBotFactory().createBot();
	sim.getBotInput().addInput( Sim::BotInput(id, 25) );
	sim.getBotInput().addInput( Sim::BotInput(id, 75) );
	
	sim.startPhase();
	for(int c=0; c<2; c++) {
		sim.step();
	}
	sim.endPhase();
	
	sim.shutdown();
	
	return 0;
}
