#ifndef _DASCRIPT__DETAIL__
#error This file should not be included from anywhere but dascript.h

// To satisfy the IDE parser
#include "dascript.h"
#endif

// Creating references
//
/**
 * @brief Inserts a new reference to \p ref in this node.
 * The referenced node must originate from the same \c DaScript.
 */
inline void DaScript::Node::createReference(Node &ref, size_t ins)
{
	if(ref.mHost == mHost) {
		if(ins >= mRefs.size())
			ins = mRefs.size();
		
		mRefs.insert(mRefs.begin()+ins, &ref);
		ref.mHasRef.insert(this);
	}
}

// Removing references
//
inline void DaScript::Node::removeReference(size_t index)
{
	if(index < getReferenceCount()) {
		Node *ref = mRefs[index];
		mRefs.erase(mRefs.begin()+index);
		ref->mHasRef.erase(this);
	}
}

inline void DaScript::Node::removeReferenceGroup(size_t is, size_t ie)
{
	if(is < getReferenceCount()) {
		ie = (ie >= getReferenceCount()) ? getReferenceCount() : ie;
		
		for(size_t i=is; i<ie; ++i)
			mRefs[i]->mHasRef.erase(this);
		mRefs.erase(mRefs.begin()+is,mRefs.begin()+ie);
	}
}

inline void DaScript::Node::removeReferenceGroup(const std::string &id)
{
	for(size_t i=0; i<getReferenceCount(); ++i) {
		if(mRefs[i]->getId() == id)
			removeReference(i);
	}
}

inline void DaScript::Node::clearReferences()
{
	for(size_t i=0; i<getReferenceCount(); ++i)
		mRefs[i]->mHasRef.erase(this);
	mRefs.clear();
	
	for(NodeSet::iterator i=mHasRef.begin(); i!=mHasRef.end(); ++i) {
		size_t index = (*i)->indexOfRef(*this);
		(*i)->mRefs.erase((*i)->mRefs.begin()+index);
	}
	mHasRef.clear();
}

// Other
//
inline size_t DaScript::Node::nextIndexOfRef(const std::string &id,
	size_t index) const
{
	index++;
	for(;index<getReferenceCount(); ++index) {
		if(getReference(index).getId() == id)
			return index;
	}
	
	return NoPos;
}

inline size_t DaScript::Node::indexOfRef(const Node &ptr) const
{
	for(size_t i=0;i<getReferenceCount(); ++i) {
		if(&getReference(i) == &ptr)
			return i;
	}
	
	return NoPos;
}

inline const DaScript::Node& DaScript::Node::getReferencedNode(
	size_t index) const
{
	index -= mNodes.size();
	
	// Iterate through all references until a suitable
	// match is found.
	for(size_t ri=0; ri<mRefs.size();ri++) {
		size_t nodeCount = mRefs[ri]->getNodeCount(true);
		if(index < nodeCount)
			return mRefs[ri]->getNode(index);
		else
			index -= nodeCount;
	}
	
	return getEmpty();
}
