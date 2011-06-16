#include "Simulation.h"

namespace Sim {
	Simulation::Simulation() :
		mBotFactory(this)
		{}
	
	Simulation::~Simulation()
		{}
	
	void Simulation::startup(const Configuration &config)
	{
		this->config = config;
	}
	
	void Simulation::shutdown()
	{
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
