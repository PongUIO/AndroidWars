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

/**
 * @brief Creates and returns an iterator to the first node with the given ID.
 * 
 * @param id The identifier to look for, this is ignored if empty.
 * @param useRef Takes references into consideration if true.
 */
inline Nepeta::Iterator Nepeta::Node::begin(const std::string &id,
	bool useRef) const
{
	Iterator iter;
	
	iter.index = Nepeta::NoPos;
	iter.absIndex = Nepeta::NoPos;
	iter.curRef = Nepeta::NoPos;
	iter.endRef = useRef ? getReferenceCount() : 0;
	
	next(iter,id);
	return iter;
}

/**
 * @brief Creates an iterator for the first reference that matches the given ID.
 * 
 * The iterator is constrained to the first reference that matches the given
 * ID, which may be empty to match the first reference with any nodes.
 * 
 * This allows for a form of multi-node inheritance, which may be useful when
 * data readers look through all nodes matching a given id, and overriding
 * such node-groups is desired.
 */
inline Nepeta::Iterator Nepeta::Node::beginRef(const std::string &id) const
{
	Iterator it = begin(id,true);
	
	it.endRef = it.curRef+1;
	return it;
}
 
/**
 * @brief Iterates an iterator to the next element.
 * 
 * The iterator will seek forward to the next valid node (potentially only
 * if their identifier match the \p id parameter). Iteration stops at the
 * point where \c hasNext() returns false.
 */
inline void Nepeta::Node::next(Iterator &it, const std::string &id) const
{
	do {
		it.index++;
		it.absIndex++;
		
		size_t curSize;
		do {
			curSize = (
				it.curRef==Nepeta::NoPos ?
					getNodeCount(false) :
					getReference(it.curRef).getNodeCount(true)
			);
			if(it.index >= curSize) {
				it.curRef++;
				it.index = 0;
				
				curSize = getReference(it.curRef).getNodeCount(true);
			}
			
		} while(it.index >= curSize && it.curRef<it.endRef);
	} while(!id.empty() && getIterNode(it).getId() != id && hasNext(it));
}

/**
 * @brief Returns true if an iterator has more elements to iterate through.
 * 
 * If \c hasNext() returns true, a call to \c next() is valid.
 */
inline bool Nepeta::Node::hasNext(const Iterator &it) const
{
	return it.curRef==Nepeta::NoPos || it.curRef<it.endRef;
}

/**
 * @brief Returns the node pointed to by the given iterator.
 *
 * If the iterator is outside of the iterating space, the returned
 * node is the empty node.
 */
inline const Nepeta::Node &Nepeta::Node::getIterNode(
	const Iterator &it) const
{
	return
		(it.curRef==Nepeta::NoPos ?
			getNode(it.index,false) :
			getReference(it.curRef).getNode(it.index,true)
		);
}
