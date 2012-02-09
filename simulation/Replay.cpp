#include "Simulation.h"
#include "Replay.h"

namespace Sim {
	
	void ReplayManager::startup()
	{
		mPhaseSaveInterval = -1;
		mPresentPhase = 0;
	}
	
	void ReplayManager::shutdown()
	{
	}
	
	void ReplayManager::prepareSim()
	{
		createStateSnapshot();
		mPresentPhaseSave = Save(mPhaseSaves.back());
	}
	
	/**
	 * Creates an input snapshot for the current phase.
	 */
	void ReplayManager::startPhase()
	{
		createInputSnapshot();
	}
	
	/**
	 * Ends a simulation phase for the replay manager.
	 * 
	 * @param finalize If true, all future snapshots are discarded and
	 * \c mPresentPhase is updated. This essentially means "make the current
	 * phase the present".
	 */
	void ReplayManager::endPhase(bool finalize)
	{
		createStateSnapshot();
		
		if(finalize) {
			mPresentPhase = mSim->getCurPhase();
			if(getMostRecentPhaseSave(mPresentPhase) < mPresentPhase)
				mPresentPhaseSave = mSim->save();
			else
				mPresentPhaseSave = Save(mPhaseSaves[mPresentPhase]);
			
			// Determine cutoff, the sizes are incremented by one to
			// adjust off-by-one indexes
			
			// All phase saves are kept up to and including the present phase
			uint32_t cutoffSize = (mPresentPhase / mPhaseSaveInterval) +1;
			if(mPhaseSaves.size() > cutoffSize)
				mPhaseSaves.resize(cutoffSize);
			
			// All input saves are kept up to, but not including, the present
			// phase.
			uint32_t inputCutoffSize = mPresentPhase;
			if(mPhaseInput.size() > inputCutoffSize)
				mPhaseInput.resize(inputCutoffSize);
		}
	}
	
	/**
	 * Loads the "present" time for the simulation.
	 */
	void ReplayManager::gotoPresent()
	{
		mSim->load(mPresentPhaseSave);
	}
	
	/**
	 * Creates an input snapshot for the current phase.
	 * An input snapshot is performed just as \c Simulation::startPhase() is
	 * called.
	 * 
	 * Snapshots are only created for the present and future phases, they
	 * are never overwritten.
	 */
	void ReplayManager::createInputSnapshot()
	{
		if(mSim->getCurPhase() < mPresentPhase)
			return;
		
		mPhaseInput.push_back( Save() );
		Save::FilePtr fp = Save::FilePtr(mPhaseInput.back());
		
		// Save the input buffer
		mSim->getInput().save(fp);
	}
	
	/**
	 * Creates a full-state snapshot of the current simulation state.
	 * This is only done if this phase is a snapshot-phase.
	 * 
	 * A full state snapshot is requested at two events:
	 * \c Simulation::prepareSim() and \c Simulation::endPhase().
	 * 
	 * No snapshots are created for past phases, as they are considered
	 * finalized.
	 */
	void ReplayManager::createStateSnapshot()
	{
		if(mSim->getCurPhase() < mPresentPhase)
			return;
		
		if( (mSim->getCurPhase() % mPhaseSaveInterval) == 0 ) {
			mPhaseSaves.push_back(mSim->save());
		}
	}
	
	/**
	 * Rewinds the simulation to a state in the replay (if possible).
	 * The state is given by a phase number and step within the phase.
	 * 
	 * Internally, this causes the simulation to end up in the exact state
	 * as if processed normally. This means that at the beginning of each
	 * phase (\c Simulation::getCurPhaseStep() == 0),
	 * \c Simulation::startPhase() is called, and from there
	 * \c Simulation::step() is called. If there are no more steps in the
	 * phase after this (step >= \c Configuration::phaseLength), then
	 * \c Simulation::endPhase() is called.
	 * 
	 * @note If the parameter step==0, then \c Simulation::startPhase() is not
	 * called for the current phase.
	 * 
	 * @return False if no rewind took place (no replay data available), else
	 * true.
	 */
	bool ReplayManager::rewind(uint32_t phase, uint32_t step)
	{
		if(mPhaseSaves.size() == 0)
			return false;
		
		// Load the world state
		mSim->load( mPhaseSaves[getMostRecentPhaseSave(phase)] );
		
		// Run phases until reaching the target
		bool hasStarted = false;
		bool hasIgnoredFirstInput=false;
		while( mSim->getCurPhase() < phase ||
			(mSim->getCurPhase() >= phase && mSim->getCurPhaseStep() < step) ) {
			
			// Start the phase if necessary
			if( !hasStarted ) {
				if(hasIgnoredFirstInput)
					loadCurPhaseInput();
				else
					hasIgnoredFirstInput=true;
				mSim->startPhase();
				hasStarted = true;
			}
			
			// Run a step
			mSim->step();
			
			// End the phase if necessary
			if(!mSim->hasPhaseStep()) {
				mSim->endPhase(false);
				hasStarted = false;
			}
		}
		
		return true;
	}
	
	/**
	 * Loads any stored input for this phase. Intended for use
	 * after a call to rewind().
	 * 
	 * @note To be called just before \c Simulation::startPhase().
	 */
	void ReplayManager::loadCurPhaseInput()
	{
		if(mSim->getCurPhase() < mPhaseInput.size()) {
			Save &inputData = mPhaseInput[mSim->getCurPhase()];
			Save::FilePtr fp = Save::FilePtr(inputData);
			
			mSim->getInput().load(fp);
		}
	}
	
	/**
	 * Translates a real time unit to a phase and step. The time is rounded
	 * down to the closest phase/step pair.
	 * Utility function to simplify time to phase/step translation.
	 */
	bool ReplayManager::rewind(double timeUnit)
	{
		const Configuration &cfg = mSim->getConfig();
		double phaseTime = cfg.stepTime*cfg.phaseLength;
		
		uint32_t phase = timeUnit / phaseTime;
		uint32_t step = (fmod(timeUnit, phaseTime)/phaseTime)*cfg.phaseLength;
		
		return rewind(phase,step);
	}
	
	uint32_t ReplayManager::getMostRecentPhaseSave(uint32_t targetPhase)
	{	return targetPhase / mPhaseSaveInterval; }
	
	/**
	 * Stores the replay into a savestate.
	 */
	Save ReplayManager::saveReplay()
	{
		Save save;
		Save::FilePtr fp = Save::FilePtr(save);
		
		fp.writeInt<uint32_t>(mPhaseSaves.size());
		for(uint32_t i=0; i<mPhaseSaves.size(); i++)
			fp.writeSave(mPhaseSaves[i]);
		
		fp.writeInt<uint32_t>(mPhaseInput.size());
		for(uint32_t i=0; i<mPhaseInput.size(); i++)
			fp.writeSave(mPhaseInput[i]);
		
		fp.writeInt<uint32_t>(mPhaseSaveInterval);
		
		return save;
	}
	
	/**
	 * Loads a replay from the given savefile.
	 */
	void ReplayManager::loadReplay(Save& save)
	{
		uint32_t size;
		Save::FilePtr fp = Save::FilePtr(save);
		
		size = fp.readInt<uint32_t>();
		for(uint32_t i=0; i<size; i++)
			mPhaseSaves.push_back( fp.readSave() );
		
		size = fp.readInt<uint32_t>();
		for(uint32_t i=0; i<size; i++)
			mPhaseInput.push_back( fp.readSave() );
		
		mPhaseSaveInterval = fp.readInt<uint32_t>();
	}
}
