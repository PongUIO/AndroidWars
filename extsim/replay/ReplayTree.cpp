#include "ReplayTree.h"

namespace exts {
	// ReplayNode
	// 
	// 
	
	// ReplayBranch
	//
	//
	ReplayBranch::~ReplayBranch()
	{
		for(NodeVec::iterator i=mNodes.begin(); i!=mNodes.end(); ++i)
			delete *i;
	}
	
	/**
	 * @brief Gets the node corresponding to the given phase.
	 * 
	 * The node is retrieved using the following rules:
	 * - If the \c phase exceeds the number of stored nodes, 0 is returned
	 * - If the \c phase is within the current branch, then the corresponding
	 * 	node from this branch is retrieved.
	 * - If the \c phase is less than the offset, then the corresponding
	 * 	node from the parent branch is retrieved.
	 * 
	 * @return A valid \c ReplayNode, or 0 if no node exists for the given
	 * phase.
	 */
	const ReplayNode *ReplayBranch::getNode(size_t phase) const {
		if(phase >= mOffset)
			return (phase-mOffset)<getNodeCount() ? mNodes[phase-mOffset] : 0;
		else if(mParent)
			return mParent->getNode(phase);
		else
			return 0;
	}
	
	ReplayNode* ReplayBranch::getNode(size_t phase)
	{	return const_cast<ReplayNode*>(getNode(phase) ); }

	
	void ReplayBranch::clearRefBranches()
	{
		for(BranchVec::iterator i=mRefBranches.begin();
		i!=mRefBranches.end(); ++i) {
			(*i)->mParent = mParent;
		}
	}
	
	void ReplayBranch::removeRef(const ReplayBranch* ref)
	{
		for(BranchVec::iterator i=mRefBranches.begin();
		i!=mRefBranches.end(); ++i) {
			if(*i == ref) {
				mRefBranches.erase(i);
				break;
			}
		}
	}

	
	// ReplayTree
	// 
	// 
	ReplayTree::ReplayTree() : mRoot(new ReplayNode(0,0))
	{
	}

	ReplayTree::~ReplayTree()
	{
		if(mRoot)
			delete mRoot;
	}
}
