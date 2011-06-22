#include "Simulation.h"
#include "Sync.h"

namespace Sim {
	Simulation::Simulation() :
		mStateActive(this),
		mStateCopy(this),
		mData()
		{}
	
	Simulation::~Simulation()
		{}
	
	void Simulation::startup(const Configuration &config)
	{
		this->config = config;
		
		mStateCopy.startup();
		mStateActive.startup();
		mData.startup();
	}
	
	void Simulation::shutdown()
	{
		mStateActive.shutdown();
		mStateCopy.shutdown();
		mData.shutdown();
	}
	
	void Simulation::prepareSim()
	{
		mStateCopy.copyState(mStateActive);
	}
	
	void Simulation::startPhase()
	{
		mCurPhaseStep = 0;
		
		mStateActive.startPhase();
	}
	
	void Simulation::step()
	{
		mStateActive.step(config.stepTime);
		
		mCurPhaseStep++;
	}
	
	void Simulation::endPhase()
	{
		mStateActive.endPhase();
	}
	
	void Simulation::rewindPhase()
	{
		mStateActive.copyState(mStateCopy);
	}
	
	void Simulation::finalizePhase()
	{
		mStateCopy.copyState(mStateActive);
	}
	
	uint32_t Simulation::checksumData()
	{
		Sync sync;
		mData.checksum(sync);
		return sync.sum();
	}
	
	uint32_t Simulation::checksumSim()
	{
		return mStateActive.checksum();
	}
	
}
