#ifndef REPLAYTREE_H
#define REPLAYTREE_H

#include <vector>

#include "../simulation/Save.h"

namespace ExtS {
	class ReplayBranch;
	class ReplayTree;
	
	class ReplayNode {
		public:
			ReplayNode() : mSave() {}
			
			const Sim::Save &getData() const { return mSave; }
			
		private:
			Sim::Save mSave;
	};
	
	/**
	 *
	 */
	class ReplayBranch {
		public:
			const ReplayNode *getNode(size_t phase);
			ReplayBranch *getTopBranch();
			
			size_t getNodeCount() const { return mNodes.size(); }
			
		private:
			ReplayBranch(ReplayBranch *parent=0) : mParent(parent) {}
			
			void clearRefBranches();
			
			typedef std::vector<ReplayNode*> NodeVec;
			typedef std::vector<ReplayBranch*> BranchVec;
			
			NodeVec mNodes;
			BranchVec mRefBranches;
			
			size_t mOffset;
			ReplayBranch *mParent;
			
			friend class ReplayTree;
	};
	
	/**
	 * @brief Holds a branching tree of alternate timelines for phases.
	 * 
	 * 
	 */
	class ReplayTree {
		public:
			ReplayTree();
			~ReplayTree();
			
			const ReplayBranch *createBranch(size_t index=0)
				const;
			const ReplayBranch *getBranch(size_t index) const;
			void deleteBranch(size_t index);
			
			size_t getBranchCount() const;
			
		private:
			typedef std::vector<ReplayBranch*> BranchVec;
			BranchVec mBranches;
			
			
	};
}

#endif
