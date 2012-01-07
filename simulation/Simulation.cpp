#include "Simulation.h"
#include "Save.h"

namespace Sim {
	Simulation::Simulation() :
		mStateActive(this),
		mData(this),
		mReplay(this)
		{}
	
	Simulation::~Simulation()
		{}
	
	void Simulation::startup(const Configuration &config)
	{
		this->config = config;
		
		mData.startup();
		clear();
		mReplay.startup();
	}
	
	void Simulation::shutdown()
	{
		mReplay.shutdown();
		mStateActive.shutdown();
		mData.shutdown();
	}
	
	/**
	 * Erase the current simulation world state.
	 * 
	 * @note This puts the world in a state as if \c startup() was just called,
	 * as such \c prepareSim still has to be called.
	 */
	void Simulation::clear()
	{
		mStateActive = State(this);
		mStateActive.startup();
	}
	
	void Simulation::prepareSim()
	{
		mReplay.prepareSim();
	}
	
	/**
	 * Starts a new simulation phase.
	 * 
	 * \param doTrialRun If false, all steps will be reverted when
	 * \c endPhase() is called.
	 */
	void Simulation::startPhase()
	{
		mReplay.startPhase();
		
		mStateActive.startPhase();
	}
	
	void Simulation::step()
	{
		mStateActive.step(config.stepTime);
	}
	
	/**
	 * Ends a simulation phase.
	 * 
	 * @param finalize See \c ReplayManager::endPhase().
	 */
	void Simulation::endPhase(bool finalize)
	{
		mStateActive.endPhase();
		
		mReplay.endPhase(finalize);
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
		save(sync);
		return sync.checksum();
	}
	
	/**
	 * Saves the simulation state along with some other simulation settings.
	 * This must only be called outside of \c startPhase()/\c endPhase(), as
	 * doing so is undefined behaviour.
	 * 
	 * Will not save anything in \c Data.
	 */
	Save Simulation::save()
	{
		Save tmp;
		Save::FilePtr fp = Save::FilePtr(tmp);
		save(fp);
		
		return tmp;
	}
	
	void Simulation::save(Save::BasePtr& fp)
	{
		mStateActive.save(fp);
	}
	
	/**
	 * Loads a simulation state from a corresponding \c Simulation::save()
	 * state.
	 * 
	 * \param saveData The savestate to load from.
	 */
	void Simulation::load(Save& saveData)
	{
		Save::FilePtr fp = Save::FilePtr(saveData);
		
		mStateActive.load(fp);
	}
}
