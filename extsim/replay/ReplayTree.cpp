#include <boost/bind.hpp>

#include "ReplayTree.h"

namespace exts {
	// ReplayNode
	// 
	// 
	ReplayNode::ReplayNode(ReplayTree& mgr, Sim::IdType id, ReplayNode* parent) :
		mTree(mgr), mId(id),
		mParent(parent), mChildren(), mDepth(parent ? parent->getDepth()+1 : 0)
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
	
	void ReplayNode::save(Sim::Save::BasePtr& fp) const
	{
		for(uint32_t i=0; i<NtMax; ++i)
			fp.writeSave(mSave[i]);
	}
	
	void ReplayNode::load(Sim::Save::BasePtr& fp)
	{
		for(uint32_t i=0; i<NtMax; ++i)
			mSave[i] = fp.readSave();
	}
	
	// ReplayTree
	// 
	// 
	ReplayTree::ReplayTree() : mRoot(0)
	{
		mRoot = makeNode(0);
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
	
	void ReplayTree::saveNode(Sim::Save::BasePtr& fp, const ReplayNode *node) const
	{
		if(node) {
			fp << true;
			fp << *node;
		} else {
			fp << false;
		}
	}
	
	void ReplayTree::loadNode(Sim::Save::BasePtr& fp, ReplayNode*& node)
	{
		bool hasNode;
		fp >> hasNode;
		
		Sim::IdType nextId = mNodes.size()-1;
		
		if(hasNode) {
			node = new ReplayNode(*this, nextId, 0);
			fp >> *node;
		} else {
			mFreeNodes.push(nextId);
		}
	}
	
	void ReplayTree::save(Sim::Save::BasePtr& fp) const
	{
		fp << mRoot->getId();
		fp.writeCtrF(mNodes, boost::bind(&ReplayTree::saveNode, *this, _1, _2));
	}
	
	void ReplayTree::load(Sim::Save::BasePtr& fp)
	{
		Sim::IdType rootId;
		fp >> rootId;
		fp.readCtrF(mNodes, boost::bind(&ReplayTree::loadNode, this, _1, _2));
	}
}
