#include "ReplayTree.h"

namespace ExtS {
	// ReplayNode
	// 
	// 
	
	// ReplayBranch
	// 
	// 
	const ReplayNode *ReplayBranch::getNode(size_t phase) {
		if(phase >= mOffset)
			return (phase-mOffset)<getNodeCount() ? mNodes[phase-mOffset] : 0;
		else if(mParent)
			return mParent->getNode(phase);
		else
			return 0;
	}
	
	ReplayBranch* ReplayBranch::getTopBranch()
	{
		ReplayBranch *b = this;
		while(b->mParent)
			b = b->mParent;
		
		return b;
	}
	
	void ReplayBranch::clearRefBranches()
	{
		for(BranchVec::iterator i=mRefBranches.begin();
		i!=mRefBranches.end(); ++i) {
			(*i)->mParent = mParent;
		}
	}
	
	// ReplayTree
	// 
	// 
	ReplayTree::ReplayTree()
	{
		mBranches.push_back(new ReplayBranch());
	}

	ReplayTree::~ReplayTree()
	{
		for(size_t i=0; i<getBranchCount(); ++i)
			delete mBranches[i];
	}
	
	const ReplayBranch* ReplayTree::createBranch(size_t index) const
	{
		if(index >= getBranchCount())
			index = 0;
		
		ReplayBranch *branch = new ReplayBranch();
		
	}

}
