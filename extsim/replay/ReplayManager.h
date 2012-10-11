#ifndef EXTSIM_REPLAYMANAGER_H
#define EXTSIM_REPLAYMANAGER_H

#include <list>

#include "../ExtModule.h"
#include "ReplayTree.h"

namespace exts {
	class ExtSim;
	
	/**
	 * @brief Keeps track of the simulation's history
	 * 
	 */
	class ReplayManager : public ExtModule {
		public:
			typedef std::list<const ReplayNode*> ReplayList;
			
			ReplayManager(ExtSim &esim);
			~ReplayManager();
			
			void startup() {}
			void shutdown() {}
			
			void selectBranch(Sim::IdType id);
			void replay(uint32_t phase, uint32_t step);
			void replay(double timeUnit);
			void loadPhaseInput(ReplayList &src);
			void stepReplay(ReplayList &path, uint32_t numStep=1);
			
			void gotoActive();
			
			void commit();
			void commitNewBranch();
			
			const ReplayNode *buildPathToSimSave(ReplayList& path, Sim::IdType nodeId);
			
			const ReplayTree &getReplayTree() const { return mTree; }
			
			const ReplayNode *getActiveNode() const
			{ return mTree.getNode(mActiveNode); }
		private:
			ReplayNode *getActiveNodeI()
			{ return mTree.getNode(mActiveNode); }
			
			ReplayNode *findNodeForPhase(uint32_t phase);
			
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
