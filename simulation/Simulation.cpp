#include "Simulation.h"

namespace Sim {
	Simulation::Simulation() :
		mBotFactory(this),
		mWorld(this)
		{}
	
	Simulation::~Simulation()
		{}
	
	void Simulation::startup(const Configuration &config)
	{
		this->config = config;
		
		mWorld.startup();
	}
	
	void Simulation::shutdown()
	{
		mWorld.shutdown();
	}
	
	void Simulation::startPhase()
	{
		mCurPhaseStep = 0;
		
		mBotFactory.startPhase();
	}
	
	void Simulation::step()
	{
		mBotFactory.step(0.01);
		
		mCurPhaseStep++;
	}
	
	void Simulation::endPhase()
	{
		mBotFactory.endPhase();
	}
	
}
