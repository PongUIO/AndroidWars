#include "Simulation.h"
#include "Save.h"

#include <stdio.h>
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
		
		mCurPhase = 0;
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
		mStateCopy.copyState(mStateActive);
		
		mCurPhase++;
	}
	
	void Simulation::rewindPhase()
	{
		mStateActive.copyState(mStateCopy);
	}
	
	uint32_t Simulation::checksumData()
	{
		Save dummy;
		Save::SyncPtr sync = Save::SyncPtr(dummy);
		mData.checksum(sync);
		return sync.checksum();
	}
	
	uint32_t Simulation::checksumSim()
	{
		Save save;
		mStateActive.save(save);
		return save.checksum();
	}
	
}
