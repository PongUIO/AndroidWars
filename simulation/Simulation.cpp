#include "Simulation.h"
#include "Save.h"

namespace Sim {
#define _SIM_X(type) \
	template<> type &Simulation::getComponent() \
	{ return get##type(); }
	_SIM_X_SIMULATION_COMPONENTS
#undef _SIM_X
	
	Simulation::Simulation() :
		mState(this),
		mData(this)
		{}
	
	Simulation::~Simulation()
		{}
	
	void Simulation::startup()
	{
		mData.startup();
		clear();
	}
	
	void Simulation::shutdown()
	{
		mState.shutdown();
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
		mState = State(this);
		mState.startup(mConfiguration.phaseLength);
	}
	
	void Simulation::prepareSim()
	{
	}
	
	/**
	 * Starts a new simulation phase.
	 */
	void Simulation::startPhase()
	{
		mState.startPhase();
	}
	
	void Simulation::step()
	{
		mState.step(getConfiguration().stepTime);
	}

	
	/**
	 * Ends a simulation phase.
	 * 
	 * @param finalize See \c ReplayManager::endPhase().
	 */
	void Simulation::endPhase()
	{
		mState.endPhase();
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
		mState.save(fp);
	}
	
	/**
	 * Loads a simulation state from a corresponding \c Simulation::save()
	 * state.
	 * 
	 * \param saveData The savestate to load from.
	 */
	void Simulation::load(const Save& saveData)
	{
		Save::FilePtr fp = Save::FilePtr(saveData);
		load(fp);
	}
	
	void Simulation::load(Save::BasePtr& fp)
	{
		mState.load(fp);
	}

}
