#ifndef EXTSIM_REPLAYMANAGER_H
#define EXTSIM_REPLAYMANAGER_H

#include "ReplayTree.h"

namespace exts {
	class ExtSim;
	
	/**
	 * @brief Keeps track of the simulation's history
	 */
	class ReplayManager {
		public:
			ReplayManager(ExtSim &esim);
			~ReplayManager();
			
			void selectBranch();
			
			const ReplayTree &getReplayTree() const { return mTree; }
			
		private:
			/// @name Configuration
			//@{
				/// How many phases between each full \c Simulation copy.
				uint32_t mPhaseSaveInterval;
				
				/// If true, the replay manager will make a full
				/// \c Simulation copy for a branch
				bool mDoBranchSave;
				
				/// Stores the branching tree of timelines
				ReplayTree mTree;
			//@}
			
			ExtSim *mExtSim;
	};
}

#endif
