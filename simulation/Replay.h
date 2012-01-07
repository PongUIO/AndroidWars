#ifndef SIM_REPLAY_H
#define SIM_REPLAY_H

#include <vector>
#include <stdint.h>

#include "Save.h"
#include "StateObj.h"

namespace Sim {
	class Simulation;
	
	/**
	 * @brief Contains a replay of the simulation.
	 * 
	 * This makes it possible to go back or forward in time arbitrarily,
	 * without destroying the "present" time.
	 * 
	 * The replay consists of an initial savestate of the world, plus
	 * incremental saves of input per phase.
	 * 
	 * It also supports intermediary complete saves with custom phase intervals.
	 * This is to allow fast tracking. It is suggested to keep this at the
	 * default infinite when interacting, and instead generate intermediary
	 * saves after the simulation is finished or right before replay
	 * playback.
	 */
	class ReplayManager {
		public:
			ReplayManager(Simulation *sim) : mSim(sim) {}
			~ReplayManager() {}
			
			void startup();
			void shutdown();
			
			void prepareSim();
			
			Save saveReplay();
			void loadReplay(Save &save);
			
			bool rewind(double timeUnit);
			bool rewind(uint32_t phase, uint32_t step);
			
			void gotoPresent();
			void trackbackPresent(uint32_t phase);
			
		private:
			void createStateSnapshot();
			void createInputSnapshot();
			
			void startPhase();
			void endPhase(bool finalize);
			
			uint32_t getMostRecentPhaseSave(uint32_t targetPhase);
			bool hasSnapshotOf(uint32_t targetPhase);
			
			bool isFuture(uint32_t targetPhase);
			
			typedef std::vector<Save> SaveVec;
			
			SaveVec mPhaseSaves;
			SaveVec mPhaseInput;
			Save mPresentPhaseSave;
			
			uint32_t mPhaseSaveInterval;
			uint32_t mPresentPhase;
			
			Simulation *mSim;
			
			friend class Simulation;
	};
}

#endif
