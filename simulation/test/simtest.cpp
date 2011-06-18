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
	
	sim.getSideData().addSide(testSide);
	
	// Create a test bot
	Sim::Bot::Config botCfg;
	botCfg.side = 0;
	botCfg.pos = Sim::Vector(0,0);
	uint32_t id = sim.getBotFactory().createBot( botCfg );
	
	// Send some input to this bot
	Sim::BotInput bi;
	bi.botId = id;
	bi.stepCount = 20;
	bi.type = Sim::BotInput::Move;
	
	Sim::BotInput::MoveP &biM = bi.move;
	
	biM.dir = Sim::Vector(1,0);
	sim.getBotInput().addInput( bi );
	
	biM.dir = Sim::Vector(-1,0);
	sim.getBotInput().addInput( bi );
	
	// Run a test phase
	sim.startPhase();
	while( sim.hasPhaseStep() )
		sim.step();
	sim.endPhase();
	
	// Shutdown the simulation
	sim.shutdown();
	
	return 0;
}
