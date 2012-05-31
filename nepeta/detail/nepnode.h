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

// Creating nodes
//
/**
 * @brief Creates a new child node with the given id.
 * 
 * If \p ins is specified, this is the index to insert the new node at.
 * All nodes with an index >= \p ins are one up.
 */
inline Nepeta::Node &Nepeta::Node::createNode(
	const std::string &id, size_t ins, size_t line, size_t col) {
	Node *node = new Node(mHost,this,id, line, col);
	
	if(ins > mNodes.size())
		ins = mNodes.size();
	mNodes.insert(mNodes.begin()+ins, node);
	
	return *node;
}

// Retrieving nodes
//
/**
 * @brief Retrieves the child node at the given index.
 * 
 * @param useRef If true, references are taken into consideration.
 * @return The node corresponding to the given index, or the empty node
 * if the index is invalid.
 */
inline const Nepeta::Node &Nepeta::Node::getNode(size_t index, bool useRef)
const {
	return (index<getNodeCount(false)) ?
		*mNodes[index] :
		(useRef ? getReferencedNode(index) : getEmpty());
}

// Removing nodes
//
/**
 * @brief Removes the node at the given index.
 * 
 * Node removal does not take into consideration references.
 */
inline void Nepeta::Node::removeNode(size_t index) {
	
	if(index < getNodeCount(false)) {
		delete &getNode(index,false);
		mNodes.erase(mNodes.begin()+index);
	}
}

/**
 * @brief Removes a group of child nodes.
 * 
 * All nodes from \p is and just before \p ie are removed.
 */
inline void Nepeta::Node::removeNodeGroup(size_t is, size_t ie) {
	size_t nodeCount = getNodeCount(false);
	
	if(is < nodeCount) {
		ie = (ie >= nodeCount) ? nodeCount : ie;
		
		for(size_t i=is; i<ie; ++i)
			delete &getNode(i,false);
		mNodes.erase(mNodes.begin()+is, mNodes.begin()+ie);
	}
}

/**
 * @brief Removes all child nodes with a given id.
 * 
 * All nodes in the original node, excluding all references, that have
 * the given identifier are removed.
 */
inline void Nepeta::Node::removeNodeGroup(const std::string &id) {
	for(size_t i=indexOf(id,false); i!=NoPos; i=nextIndexOf("",i,false)) {
		removeNode(i);
		i--;
	}
}

/**
 * @brief Clears out all child nodes from the node.
 */
inline void Nepeta::Node::clearNodes() {
	for(NodeVector::iterator i=mNodes.begin();
	i!=mNodes.end(); i++)
		delete (*i);
	mNodes.clear();
}


// Other
//
/**
 * @brief Returns the index that corresponds to a given node pointer.
 * 
 * @param useRef If true, references are taken into consideration.
 */
inline size_t Nepeta::Node::indexOf(const Node &ptr, bool useRef) const
{
	size_t nodeCount = getNodeCount(useRef);
	for(size_t i=0; i<nodeCount; ++i) {
		if(&getNode(i,useRef) == &ptr)
			return i;
	}
	
	return NoPos;
}

/**
 * @brief Returns the next index after \p index that contains the given id.
 * 
 * @return The index of the next node, or \c NoPos if none exist.
 */
inline size_t Nepeta::Node::nextIndexOf(const std::string &id,
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
 * @brief Retrieves the number of child nodes in this node.
 * 
 * @param useRef If true, references are taken into
 * consideration.
 * @warning Try to call this as infrequently as possible,
 * as it runs in linear time when references are
 * counted. If any circular references exist, this function
 * never terminates.
 */
inline size_t Nepeta::Node::getNodeCount(bool useRef) const {
	size_t s=mNodes.size();
	if(useRef) {
		for(NodeVector::const_iterator i=mRefs.begin(); i!=mRefs.end(); ++i)
			s+=(*i)->getNodeCount();
	}
	return s;
}
