#ifndef EXTSIM_REPLAYMANAGER_H
#define EXTSIM_REPLAYMANAGER_H

#include "ReplayTree.h"

namespace exts {
	class ExtSim;
	
	/**
	 * @brief Keeps track of the simulation's history
	 * 
	 */
	class ReplayManager {
		public:
			ReplayManager(ExtSim &esim);
			~ReplayManager();
			
			void selectBranch(Sim::IdType id);
			void replay(uint32_t phase, uint32_t step);
			void replay(double timeUnit);
			void step(uint32_t numStep=1);
			
			void gotoActive();
			
			void commit();
			void commitNewBranch();
			
			const ReplayTree &getReplayTree() const { return mTree; }
			
			const ReplayNode *getActiveNode() const
			{ return mTree.getNode(mActiveNode); }
			ReplayNode *getActiveNode()
			{ return mTree.getNode(mActiveNode); }
		private:
			/// @name Data
			//@{
				ExtSim &mExtSim;
				
				/// Stores the branching tree of timelines
				ReplayTree mTree;
				
				/// Currently active branch node
				Sim::IdType mActiveNode;
			//@}
			
			/// @name Configuration
			//@{
				/// How many phases between each full \c Simulation copy.
				uint32_t mPhaseSaveInterval;
			//@}
	};
}

#endif
