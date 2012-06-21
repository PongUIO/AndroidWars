#include "ReplayTree.h"

namespace ExtS {
	// ReplayNode
	// 
	// 
	
	// ReplayBranch
	//
	//
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
	ReplayTree::ReplayTree()
	{
		mBranches.push_back(new ReplayBranch());
	}

	ReplayTree::~ReplayTree()
	{
		for(size_t i=0; i<getBranchCount(); ++i)
			delete mBranches[i];
	}
	
	/// Gets a \c ReplayBranch corresponding to the given index
	const ReplayBranch* ReplayTree::getBranch(size_t index) const
	{	return (index<getBranchCount()) ? mBranches[index] : 0; }
	
	/// @copydoc getBranch
	ReplayBranch* ReplayTree::getBranch(size_t index)
	{	return const_cast<ReplayBranch*>(getBranch(index)); }

	
	/**
	 * @brief Creates a new timeline branch.
	 * 
	 * If an index is specified, the new branch uses the branch at that
	 * index as its parent.
	 */
	size_t ReplayTree::createBranch(size_t index)
	{
		// Get parent
		if(index >= getBranchCount())
			index = 0;
		ReplayBranch *parent = getBranch(index);
		
		// Create branch
		ReplayBranch *branch = new ReplayBranch(parent);
		mBranches.push_back(branch);
		
		// Update the parent's back-references
		parent->updateRef(branch);
		
		// Return the index of the new branch
		return mBranches.size()-1;
	}
	
	/**
	 * @brief Deletes a branch from the tree, if possible.
	 * 
	 * A branch can only be deleted if it is a leaf, meaning no other
	 * branch has this as their parent.
	 * 
	 * A special exception exists for branch 0 (the main timeline),
	 * which may never be deleted.
	 * 
	 * @return True if the branch could and was deleted, false otherwise.
	 */
	bool ReplayTree::deleteBranch(size_t index)
	{
		// The top-level branch may never be deleted
		if(index == 0)
			return false;
		
		ReplayBranch *branch = getBranch(index);
		
		// If the index is invalid, or the branch has descendants, then
		// the branch can not be deleted
		if(!branch || branch->getDescendantCount()>0)
			return false;
		
		// Delete the branch
		mBranches.erase(mBranches.begin()+index);
		branch->getParent()->removeRef(branch);
		return true;
	}
}
