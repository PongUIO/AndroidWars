#ifndef _DASCRIPT__DETAIL__
#error This file should not be included from anywhere but dascript.h

// To satisfy the IDE parser
#include "dascript.h"
#endif

// Creating nodes
//
/**
 * @brief Creates a new node with the given id.
 * 
 * If \p ins is specified, this is the index to insert the new node at.
 * All nodes with an index >= \p ins are one up.
 */
inline DaScript::Node &DaScript::Node::createNode(
	const std::string &id, size_t ins, size_t line, size_t col) {
	Node *node = new Node(mHost,this,id, line, col);
	
	if(ins > mNodes.size())
		ins = mNodes.size();
	mNodes.insert(mNodes.begin()+ins, node);
	
	return *node;
}

// Retrieving nodes
//
inline const DaScript::Node &DaScript::Node::getNode(size_t index, bool useRef)
const {
	return (index<getNodeCount(false)) ?
		*mNodes[index] :
		(useRef ? getReferencedNode(index) : getEmpty());
}

// Removing nodes
//
inline void DaScript::Node::removeNode(size_t index) {
	
	if(index < getNodeCount(false)) {
		delete &getNode(index,false);
		mNodes.erase(mNodes.begin()+index);
	}
}

inline void DaScript::Node::removeNodeGroup(size_t is, size_t ie) {
	size_t nodeCount = getNodeCount(false);
	
	if(is < nodeCount) {
		ie = (ie >= nodeCount) ? nodeCount : ie;
		
		for(size_t i=is; i<ie; ++i)
			delete &getNode(i,false);
		mNodes.erase(mNodes.begin()+is, mNodes.begin()+ie);
	}
}

inline void DaScript::Node::removeNodeGroup(const std::string &id) {
	for(size_t i=indexOf(id,false); i!=NoPos; i=nextIndexOf(id,i,false)) {
		removeNode(i);
		i--;
	}
}

inline void DaScript::Node::clearNodes() {
	for(NodeVector::iterator i=mNodes.begin();
	i!=mNodes.end(); i++)
		delete (*i);
	mNodes.clear();
}


// Other
//
inline size_t DaScript::Node::indexOf(const Node &ptr, bool useRef) const
{
	size_t nodeCount = getNodeCount(useRef);
	for(size_t i=0; i<nodeCount; ++i) {
		if(&getNode(i,useRef) == &ptr)
			return i;
	}
	return NoPos;
}

inline size_t DaScript::Node::nextIndexOf(const std::string &id,
	size_t index, bool useRef) const {
	
	index++;
	size_t nodeCount = getNodeCount(useRef);
	for(;index<nodeCount; index++) {
		if(getNode(index,useRef).getId() == id)
			return index;
	}
	
	return NoPos;
}

/**
 * Retrieves the node count for this instance.
 * 
 * @param useRef If true, references are taken into
 * consideration.
 * @warning Try to call this as infrequently as possible,
 * as it runs in linear time when references are
 * counted. If any circular references exist, this function
 * never terminates.
 */
inline size_t DaScript::Node::getNodeCount(bool useRef) const {
	size_t s=mNodes.size();
	if(useRef) {
		for(NodeVector::const_iterator i=mRefs.begin(); i!=mRefs.end(); ++i)
			s+=(*i)->getNodeCount();
	}
	return s;
}
