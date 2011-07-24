#include "Simulation.h"
#include "Save.h"

namespace Sim {
	Simulation::Simulation() :
		mStateActive(this),
		mData(this)
		{}
	
	Simulation::~Simulation()
		{}
	
	void Simulation::startup(const Configuration &config)
	{
		this->config = config;
		
		mStateActive.startup();
		mData.startup();
	}
	
	void Simulation::shutdown()
	{
		mStateActive.shutdown();
		mData.shutdown();
	}
	
	void Simulation::prepareSim()
	{
		mStateCopy = save();
		
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
		mStateCopy = save();
		
		mCurPhase++;
	}
	
	void Simulation::rewindPhase()
	{
		Save::FilePtr fptr = Save::FilePtr(mStateCopy);
		mStateActive.load( fptr );
	}
	
	uint32_t Simulation::checksumData()
	{
		Save::SyncPtr sync = Save::SyncPtr();
		mData.save(sync);
		return sync.checksum();
	}
	
	uint32_t Simulation::checksumSim()
	{
		Save::SyncPtr sync = Save::SyncPtr();
		mStateActive.save(sync);
		return sync.checksum();
	}
	
	Save Simulation::save()
	{
		Save tmp;
		Save::FilePtr ptr = Save::FilePtr(tmp);
		mStateActive.save(ptr);
		return tmp;
	}
	
	void Simulation::load(const Sim::Save& saveData)
	{
		/// @todo Implement this
	}
}
