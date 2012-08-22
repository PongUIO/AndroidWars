#include "ReplayTree.h"

namespace exts {
	// ReplayNode
	// 
	// 
	ReplayNode::ReplayNode(ReplayTree& mgr, Sim::IdType id, ReplayNode* parent) :
		mTree(mgr), mId(id),
		mParent(parent), mChildren(), mDepth(parent->getDepth())
	{}
	
	ReplayNode::~ReplayNode()
	{}
	
	ReplayNode* ReplayNode::createBranch()
	{
		ReplayNode *tmp = mTree.makeNode(this);
		mChildren.push_back(tmp);
		return tmp;
	}
	
	/**
	 * @brief Prepares this node to be modified.
	 * 
	 * This means all children nodes will be destroyed, as any modifications
	 * to this node invalidates all children. In addition, any savedata
	 * is cleared.
	 */
	void ReplayNode::modifyNode()
	{
		// Remove all children
		for(ReplayNodeVec::iterator i=mChildren.begin(); i!=mChildren.end();
		++i) {
			mTree.freeNode(*i);
		}
		mChildren.clear();
		
		// Destroy any savedata
		for(size_t i=0; i<NtMax; ++i)
			mSave[i].clear();
	}

	
	// ReplayTree
	// 
	// 
	ReplayTree::ReplayTree() : mRoot(makeNode())
	{
	}

	ReplayTree::~ReplayTree()
	{
		freeNode(mRoot);
	}
	
	ReplayNode* ReplayTree::makeNode(ReplayNode *parent)
	{
		// Allocate node ID
		size_t id;
		if(mFreeNodes.size() > 0) {
			id = mFreeNodes.top();
			mFreeNodes.pop();
		} else {
			id = mNodes.size();
			mNodes.push_back(0);
		}
		
		ReplayNode *tmp = new ReplayNode(*this, id, parent);
		
		mNodes[id] = tmp;
		return tmp;
	}
	
	/**
	 * @brief Removes a node and all its descendants
	 */
	void ReplayTree::freeNode(ReplayNode* node)
	{
		ReplayNodeVec &children = node->getChildren();
		for(ReplayNodeVec::iterator i=children.begin(); i!=children.end();
		++i) {
			freeNode(*i);
		}
		
		mFreeNodes.push(node->getId());
		mNodes[node->getId()] = 0;
		delete node;
	}
	
}
