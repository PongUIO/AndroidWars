/**
 * This file is part of the Nepeta project.
 * 
 * Licensed under GNU LGPL (see license.txt and README)
 */
#ifndef _NEPETA__DETAIL__
#error This file should not be included from anywhere but nepeta.h

// To satisfy the IDE parser
#include "nepeta.h"
#endif

// Creating references
//
/**
 * @brief Inserts a new reference to \p ref in this node.
 * 
 * @note The referenced node must originate from the same \c Nepeta object.
 */
inline void Nepeta::Node::createReference(Node &ref, size_t ins)
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
/**
 * @brief Removes the reference corresponding to the given \p index.
 * 
 * @see removeNode
 */
inline void Nepeta::Node::removeReference(size_t index)
{
	if(index < getReferenceCount()) {
		Node *ref = mRefs[index];
		mRefs.erase(mRefs.begin()+index);
		ref->mHasRef.erase(this);
	}
}

/**
 * @brief Removes a group of references from the node.
 * 
 * @see removeNodeGroup(size_t,size_t)
 */
inline void Nepeta::Node::removeReferenceGroup(size_t is, size_t ie)
{
	if(is < getReferenceCount()) {
		ie = (ie >= getReferenceCount()) ? getReferenceCount() : ie;
		
		for(size_t i=is; i<ie; ++i)
			mRefs[i]->mHasRef.erase(this);
		mRefs.erase(mRefs.begin()+is,mRefs.begin()+ie);
	}
}

/**
 * @brief Removes all references to nodes that have the given id.
 */
inline void Nepeta::Node::removeReferenceGroup(const std::string &id)
{
	for(size_t i=0; i<getReferenceCount(); ++i) {
		if(mRefs[i]->getId() == id)
			removeReference(i);
	}
}

/**
 * @brief Removes all references from this node.
 */
inline void Nepeta::Node::clearReferences()
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
/// @internal
inline size_t Nepeta::Node::nextIndexOfRef(const std::string &id,
	size_t index) const
{
	index++;
	for(;index<getReferenceCount(); ++index) {
		if(getReference(index).getId() == id)
			return index;
	}
	
	return NoPos;
}

/// @internal
inline size_t Nepeta::Node::indexOfRef(const Node &ptr) const
{
	for(size_t i=0;i<getReferenceCount(); ++i) {
		if(&getReference(i) == &ptr)
			return i;
	}
	
	return NoPos;
}

/// @internal
inline const Nepeta::Node& Nepeta::Node::getReferencedNode(
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
