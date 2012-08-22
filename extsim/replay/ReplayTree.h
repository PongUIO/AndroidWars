#ifndef EXTSIM_REPLAYTREE_H
#define EXTSIM_REPLAYTREE_H

#include <vector>

#include "../simulation/Save.h"

namespace exts {
	class ReplayTree;
	
	/**
	 * @brief Replay tree node that contains all relevant data for a phase.
	 * 
	 * A node contains one or more \c Save objects for different aspects
	 * related to \c Simulation replays, including metadata such as the
	 * timeline used to generate the input, or full simulation state saves.
	 */
	class ReplayNode {
		public:
			/// Identifies the types of data stored in this node
			enum NodeType {
				/// Copy of the simulation state
				NtSimulation	= 0,
				/// Iterative save of raw input
				NtInput			= 1,
				/// Save of the \c TimelineManager for this phase
				NtTimeline		= 2,
				
				NtMax
			};
			
			ReplayNode(ReplayNode *parent=0, size_t depth=0);
			~ReplayNode();
			
			const Sim::Save &getData(NodeType type) const
			{ return mSave[type]; }
			Sim::Save &getData(NodeType type) { return mSave[type]; }
			
			bool isTypeSet(NodeType type) const
			{ return mSave[type].size()>0; }
			
			bool isRoot() const { return !mParent; }
			bool isBranch() const { return mChildren.size()>0; }
			bool isLeaf() const { return mChildren.size()==0; }
			
			ReplayNode *getParent() { return mParent; }
			const ReplayNode *getParent() const { return mParent; }
			
			size_t getChildrenCount() const { return mChildren.size(); }
			size_t getParentCount() const;
			
		private:
			typedef std::vector<ReplayNode*> NodeVec;
			NodeVec mChildren;
			
			ReplayNode *mParent;
			size_t mDepth;
			
			Sim::Save mSave[NtMax];
			
			friend class ReplayTree;
	};
	
	/**
	 * @brief Handles a single timeline branch.
	 * 
	 * A branch is, from an external view, a linear timeline with all
	 * relevant data for \c ExtSim to recreate a \c Simulation state
	 * perfectly.
	 */
	class ReplayBranch {
		public:
			typedef std::vector<ReplayBranch*> BranchVec;
			
			const ReplayBranch *getParent() const { return mParent; }
			ReplayBranch *getParent() { return mParent; }
			
			const BranchVec &getDescendants() const { return mRefBranches; }
			const ReplayNode *getNode(size_t phase) const;
			ReplayNode *getNode(size_t phase);
			
			size_t getNodeCount() const { return mNodes.size(); }
			size_t getDescendantCount() const { return mRefBranches.size(); }
			
		private:
			ReplayBranch(ReplayBranch *parent=0) :
				mOffset(0), mPresent(0), mParent(parent) {}
			~ReplayBranch();
			
			void clearRefBranches();
			
			void updateRef(ReplayBranch *ref) { mRefBranches.push_back(ref); }
			void removeRef(const ReplayBranch *ref);
			
			typedef std::vector<ReplayNode*> NodeVec;
			
			NodeVec mNodes;
			BranchVec mRefBranches;
			
			/// Phase offset for this branch, or the phase where nodes are
			/// local rather than inherited from parents.
			size_t mOffset;
			
			/// Marks the "present" phase, all nodes before this is
			/// defined constant/unmodifiable.
			size_t mPresent;
			
			ReplayBranch *mParent;
			
			friend class ReplayTree;
	};
	
	/**
	 * @brief Holds a branching tree of alternate timelines for phases.
	 * 
	 * The \c ReplayTree has the role of keeping distinct timelines organized
	 * in such a way that \c ExtSim can simulate several branching timelines
	 * with a common ancestry.
	 * 
	 * This is implemented by assuming each branch contain all phase-data up
	 * until its local present. Phase-data for phases before the branch
	 * branched off are transparently available as if the branch was a
	 * linear timeline.
	 */
	class ReplayTree {
		public:
			ReplayTree();
			~ReplayTree();
			
			ReplayNode *getRoot();
			
		private:
			ReplayNode *mRoot;
	};
}

#endif
