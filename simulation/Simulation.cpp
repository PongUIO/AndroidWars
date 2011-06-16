#include "Simulation.h"

namespace Sim {
	Simulation::Simulation() :
		mBotFactory(this)
		{}
	
	Simulation::~Simulation()
		{}
	
	void Simulation::startup(const Configuration &config)
	{
	}
	
	void Simulation::shutdown()
	{
	}
	
	void Simulation::startPhase()
	{
		mBotFactory.startPhase();
	}
	
	void Simulation::step()
	{
		mBotFactory.step(0.01);
	}
	
	void Simulation::endPhase()
	{
		mBotFactory.endPhase();
	}
	
}
