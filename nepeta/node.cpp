/**
 * This file is part of the Nepeta project.
 * 
 * Licensed under GNU LGPL (see license.txt and README)
 */
#include <sstream>
#include <deque>
#include <limits>

#include "nepeta.h"

/// Common empty string object
const std::string Nepeta::Node::sEmpty = "";

/// Special value marking an invalid or nonexistent index.
/// Set to the maximum value of \c size_t.
const size_t Nepeta::NoPos = std::numeric_limits<size_t>::max();

// Nepeta::Node
//
//
/**
 * @brief Detects circular references.
 * Analyses the references for this node to determine if any
 * circular references exist.
 * 
 * @return The index of the first such circular reference found, or
 * \c NoPos otherwise.
 */
size_t Nepeta::Node::getCircularReference() const {
	ConstNodeSet nset;
	return hasCircularReferenceInt(nset);
}

size_t Nepeta::Node::hasCircularReferenceInt(ConstNodeSet &nset) const {
	if(nset.count(this))
		return 0;
	
	nset.insert(this);
	
	for(size_t i=0; i<mRefs.size(); i++) {
		size_t ret = mRefs[i]->hasCircularReferenceInt(nset);
		if( ret != NoPos  )
			return i;
	}
	
	nset.erase(this);
	return NoPos;
}

/**
 * Retrieves a node in a simple manner. This is a
 * convenience wrapper around \c getNodeId() that makes
 * the returned node valid by setting its identifier.
 */
Nepeta::Node &Nepeta::Node::getNodeSimple(const std::string &id) {
	Node *tmp = &getNode(indexOf(id));
	if(tmp->isEmpty())
		tmp = &createNode(id);
	return *tmp;
}


static inline size_t readNum(const std::string &str, size_t start, size_t end)
{
	size_t val=0;
	
	for(size_t i=start; i<end; i++)
		val = val*10+str[i]-'0';
	
	return val;
}

/**
 * @brief Finds a node relative to this, given an input "directory" string
 * 
 * Tries to find the given node based on the input
 * string. The string has a format according to the
 * following mini-language, which is very similar
 * to common *nix directory traversal (parenthesis identify optional elements,
 * a '|' identify a branching either/or):
 * 
 * (/)(nodeId|..)([index])(/)...
 * 
 * Where the initial presence of a / identifies whether
 * the lookup is absolute from the root node, or its
 * absence where the lookup is relative.
 * 
 * "nodeId" is the node to lookup, and the [index] is the
 * enumerated "nodeID" in case there are several nodes
 * with the same id (starting at 1). If nodeId is
 * absent, the [index] parameter enumerates all nodes.
 * 
 * '..' indicates moving up a level, and if the [index]
 * parameter is present, it identifies the number of
 * levels to ascend.
 * 
 * @note The function is very forgiving with bad data. Reduntant data is
 * ignored, and some mistyping (especially a forgotten closing ']') is
 * silently ignored.
 */
Nepeta::Node& Nepeta::Node::lookupNode(const std::string& dir, bool useRef)
{
	size_t i=0;			// String index
	Node *base=this;	// Current node
	
	// Set absolute or relative search
	if(dir[0] == '/') {
		base=&mHost->getRoot();
		i++;
	}
	
	// Parse the lookup string and update base accordingly
	for(; i<dir.size();) {
		// Setup parameters
		size_t nodeIndex=1;
		bool doAscend=false;
		
		// Read the node ID
		size_t nodeIdStart = i;
		while(i<dir.size() && dir[i]!='/' && dir[i]!='[')
			i++;
		size_t nodeIdEnd = i;
		
		// Set mode
		if( (nodeIdEnd-nodeIdStart) == 2 &&
			dir[nodeIdStart]=='.' &&
			dir[nodeIdStart+1]=='.') {
			doAscend = true;
		}
		
		// Read index if available
		if(i<dir.size() && dir[i]=='[') {
			i++;
			size_t numStart = i;
			while(i<dir.size() && (dir[i]!=']' && dir[i]!='/'))
				i++;
			
			nodeIndex = readNum(dir,numStart,i);
		}
		
		// Parse to the next "directory"
		while(i<dir.size() && dir[i]!='/')
			i++;
		i++;
		
		if(doAscend) {
			Node *tmpParent;
			while( (nodeIndex--)>0 && (tmpParent=base->getParent()) )
				base = tmpParent;
		} else {
			std::string nodeId = dir.substr(nodeIdStart,nodeIdEnd-nodeIdStart);
			if(nodeId.empty())
				base = &base->getNode(nodeIndex-1, useRef);
			else {
				size_t j=base->indexOf(nodeId, useRef);
				size_t nodeCount = base->getNodeCount(useRef);
				while( (--nodeIndex)>0 && j<nodeCount )
					j=base->nextIndexOf(nodeId,j,useRef);
				
				base = &base->getNode(j,useRef);
			}
		}
	}
	
	return *base;
}

/**
 * @brief Finds the index for the nested @p node in this node.
 * 
 * This is essentially equivalent to:
 * @code
 * true == (&node == &this->getNode(this->getNodeIdIndex(node)))
 * @endcode
 * 
 * @return The node index if successful, or @c NoPos if not found.
 */
size_t Nepeta::Node::getNodeIdIndex(const Node& node)
{
	size_t nodeCount = getNodeCount();
	size_t count = 0;
	for(size_t i=0; i<nodeCount; i++) {
		Node &other = getNode(i);
		if(&node != &other) {
			if(node.getId() == other.getId())
				count++;
		} else
			return count;
	}
	
	return NoPos;
}

struct LookupData {
	LookupData(Nepeta::Node *n=0, size_t i=0) : node(n), idIndex(i) {}
	Nepeta::Node *node;
	size_t idIndex;
};

/**
 * @brief Constructs a string suitable for parsing by @c lookupNode.
 * 
 * If @p relativeTo is not null, the string returned is relative to that node.
 */
std::string Nepeta::Node::buildLookupString(Node *relativeTo)
{
	// Setup variables
	std::string str = "";
	
	// Build the lookup stack
	std::deque<LookupData> Lstack;
	Nepeta::Node *cur = this, *parent = this;
	while( (parent = parent->getParent()) ) {
		size_t idIndex = (parent ? parent->getNodeIdIndex(*cur) : NoPos);
		
		Lstack.push_back( LookupData(cur, idIndex) );
		cur = parent;
	}
	
	// Find the number of relative levels
	if(relativeTo) {
		std::deque<Node*> Rstack;
		size_t relLevel = 0;
		
		// Build the relative stack
		cur = relativeTo;
		parent = relativeTo;
		while( (parent = parent->getParent()) ) {
			Rstack.push_back( cur );
			cur = parent;
		}
		
		// Peel away shared directories
		while( Lstack.size()>0 && Rstack.size()>0 ) {
			Node *src = Lstack.back().node;
			Node *rel = Rstack.back();
			
			if( src != rel )
				break;
			else {
				Lstack.pop_back();
				Rstack.pop_back();
				
				relLevel++;
			}
		}
		
		// Create a relative path only if at least one relative directory is
		// detected. (relLevel = number of shared directories)
		if(relLevel > 0) {
			// (relLevel = number of directories to move up to the shared path)
			relLevel = Rstack.size();
			if(relLevel > 0)
				str += "..";
			
			if(relLevel > 1) {
				std::stringstream stream;
				stream << "[" << relLevel << "]";
				str += stream.str();
			}
		}
	}
	
	// Construct the string from the stack
	while(!Lstack.empty()) {
		LookupData &lookup = Lstack.back();
		str += "/"+lookup.node->getId();
		if(lookup.idIndex > 0) {
			std::stringstream stream;
			stream << (lookup.idIndex+1);
			str += "["+stream.str()+"]";
		}
		Lstack.pop_back();
	}
	
	return str;
}
