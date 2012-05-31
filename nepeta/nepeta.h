/**
 * This file is part of the Nepeta project.
 * 
 * Licensed under GNU LGPL (see license.txt and README)
 */
#ifndef _NEPETA_H
#define _NEPETA_H

#include <string>
#include <set>
#include <vector>
#include <map>

#include <stdint.h>

/**
 * @brief 
 */
class Nepeta {
	public:
		static const size_t NoPos;
		
		/**
		 * @brief Holds the data for a node iterator.
		 * 
		 * The actual iteration is done in the node's interface.
		 * @see Node
		 */
		struct Iterator {
			size_t index;	///< Current children node index.
			size_t absIndex;///< Absolute node index for \c getNode().
			size_t curRef;	///< Current reference. \c NoPos if original node
			
			/// Marks the index of the last reference to iterate.
			/// Iteration ends just before this index.
			size_t endRef;
		};
		
		/**
		 * @brief Nepeta node object.
		 * 
		 * Contains an identifier, an argument list, a list of nested nodes,
		 * and a list of references.
		 */
		class Node {
			private:
				typedef std::vector<Node*> NodeVector;
				typedef std::set<const Node*> ConstNodeSet;
				typedef std::set<Node*> NodeSet;
				typedef std::vector<std::string> ArgVector;
				
				Node(Nepeta *src, Node *parent=0, const std::string &id="",
					uint32_t line=0, uint32_t col=0) :
					mId(id), mLine(line), mCol(col),
					mParent(parent), mHost(src),
					
					mNodes(), mArgs(), mRefs(), mHasRef()
					{}
				~Node() { clear(); }
				
				Node(const Node &ref) :
					mId(), mLine(), mCol(), mParent(0), mHost(0),
					mNodes(), mArgs(), mRefs(), mHasRef()
				{	*this = ref; }
				
			public:
				/// @name System
				/// Manages special functions related for the
				/// node system.
				//@{
					/**
					 * @brief Returns a reference to the special "empty" node.
					 * This node always hold the same pointer value for a
					 * script object.
					 */
					Node &getEmpty() const
					{	return mHost->getEmpty(); }
					
					/**
					 * @brief Erases all data in the node recursively.
					 * After a call to this, the node contains the same data
					 * as the empty node.
					 */
					void clear() {
						setId("");
						clearArgs();
						clearNodes();
						clearReferences();
					}
				//@}
				
				/// @name Metadata interface
				/// Interface for the metadata for a node, such as reading
				/// its identifier and parent node.
				//@{
					/**
					 * @brief Returns @c true if this node is a "valid" node.
					 * A valid node is a node with its identifier set to a
					 * non-empty string.
					 */
					bool isValid() const { return !getId().empty(); }
					
					/// Returns \c true if this node is the "empty" node.
					bool isEmpty() const { return this == &mHost->mEmpty; }
					
					void setId(const std::string &id) { mId=id; }
					const std::string &getId() const { return mId; }
					
					// Constant data
					size_t getLine() const { return mLine; }
					size_t getCol() const { return mCol; }
					
					Node *getParent() { return mParent; }
					const Node *getParent() const { return mParent; }
				//@}
				
				/// @name Basic argument interface
				/// Interface for creating, accessing, and removing arguments.
				//@{
					/// Returns the number of arguments this node has.
					size_t getArgCount() const { return mArgs.size(); }
					
					/// Creates a new argument for this node.
					std::string &createArg(const std::string &str="")
					{	setArg(getArgCount(),str); return mArgs.back(); }
					
					/**
					 * @brief Sets the argument string at the given index.
					 * The argument list is resized to fit.
					 */
					void setArg(size_t index=0, const std::string &str="") {
						if(index>=getArgCount())
							mArgs.resize(index+1);
						mArgs[index] = str;
					}
					
					/**
					 * @brief Returns the argument for the given index.
					 * Out-of-bounds arguments return an empty string.
					 */
					const std::string &getArg(size_t index=0) const
					{ return (index<getArgCount()) ? mArgs[index] : sEmpty; }
					
					/**
					 * @brief Removes an argument from the node.
					 * The argument list is shifted as needed.
					 */
					void removeArg(size_t index=0) {
						if(index<getArgCount())
							mArgs.erase(mArgs.begin()+index);
					}
					
					/// Clears the argument list.
					void clearArgs() { mArgs.clear(); }
				//@}
				
				/**
				 * @name Basic node interface
				 * Interface for creating, accessing, and removing nested
				 * nodes.
				 */
				//@{
					// Creating nodes
					//
					Node &createNode(const std::string &id, size_t ins=NoPos,
						size_t line=0, size_t col=0 );
					
					// Retrieving nodes
					//
					const Node &getNode(size_t index, bool useRef=true) const;
					Node &getNode(size_t index, bool useRef=true)
					{	return const_cast<Node&>(static_cast<const Node&>(
						*this).getNode(index,useRef)); }
					
					const Node &getNode(const std::string &id) const
					{ return getNode(indexOf(id)); }
					Node &getNode(const std::string &id)
					{ return getNode(indexOf(id)); }
					
					// Removing nodes
					//
					void removeNode(size_t index);
					
					void removeNodeGroup(size_t is, size_t ie=NoPos);
					void removeNodeGroup(const std::string &id);
					
					void clearNodes();
					
					// Other
					//
					size_t indexOf(const std::string &id,bool useRef=true) const
					{ return nextIndexOf(id,NoPos,useRef); }
					size_t indexOf(const Node &ptr, bool useRef=true) const;
					
					size_t nextIndexOf(const std::string &id,
						size_t index, bool useRef=true) const;
					
					size_t getNodeCount(bool useRef=true) const;
				//@}
				
				/**
				 * @name References
				 * This set of functions manage node references. These are
				 * used to implement a form of inheritance for nodes,
				 */
				//@{
					// Creating references
					//
					void createReference(Node &ref, size_t ins=NoPos);
					
					// Retrieving references
					//
					const Node &getReference(size_t index) const
					{ return ((index<getReferenceCount()) ?
						*mRefs[index] : getEmpty()); }
					Node &getReference(size_t index)
					{	return const_cast<Node&>(static_cast<const Node&>(
						*this).getReference(index)); }
					
					// Removing references
					//
					void removeReference(size_t index);
					
					void removeReferenceGroup(size_t is, size_t ie);
					void removeReferenceGroup(const std::string &id);
					
					void clearReferences();
					
					// Other
					//
					size_t indexOfRef(const std::string &id) const
					{ return nextIndexOfRef(id,NoPos); }
					size_t indexOfRef(const Node &ptr) const;
					
					size_t nextIndexOfRef(const std::string &id,
						size_t index) const;
					
					size_t getReferenceCount() const { return mRefs.size(); }
					
					const Node &getReferencedNode(size_t index) const;
					Node &getReferencedNode(size_t index)
					{	return const_cast<Node&>(static_cast<const Node&>(
							*this).getReferencedNode(index)); }
				//@}
				
				/**
				 * @name Iterator interface
				 * Contains the interface to use iterators. Iterators are a
				 * faster alternative to \c indexOf() and \c nextIndexOf().
				 */
				//@{
					Iterator begin(const std::string &id="", bool useRef=true)
						const;
					Iterator beginRef(const std::string &id="") const;
					void next(Iterator &i, const std::string &id="") const;
					
					bool hasNext(const Iterator &it) const;
					const Node &getIterNode(const Iterator &i) const;
					Node &getIterNode(const Iterator &i)
					{	return const_cast<Node&>(static_cast<const Node&>(
							*this).getIterNode(i)); }
				//@}
				
				/**
				 * @name Extended node interface
				 * Contains functions that are more advanced or are intended
				 * to be for the convenience of the user.
				 */
				//@{
					Node &lookupNode(const std::string &dir, bool useRef=true);
					std::string buildLookupString(Node *relativeTo=0);
					size_t getCircularReference() const;
					
					Node &getNodeSimple(const std::string &id);
					const std::string &getNodeFirst(
						const std::string &nodeId) const
					{	return getNode(indexOf(nodeId)).getArg(); }
					
					size_t getNodeIdIndex(const Node &node);
				//@}
				
				/**
				 * @name Data chaining
				 * This set of functions offers a simplified system for
				 * creating or reading node data.
				 */
				//@{
					/**
					 * @brief Contains a write-chain.
					 * A write chain is a a chain of functions allowing
					 * recursive node and argument building in a simple manner.
					 */
					class WriteChain {
						public:
							WriteChain(Node &node,
								WriteChain *parent=0) :
								mNode(node), mParent(parent) {}
							
							/**
							 * @brief Template function for chained writing.
							 * This may be specialized to implement writers
							 * for different data types.
							 */
							template<class T>
							void writeArg(const T &val)
							{	createarg() = val; }
							
							/// Writes a new argument to the node
							template<class T>
							WriteChain &arg(const T &val=T())
							{	writeArg(val); return *this; }
							
							/**
							 * @brief Creates a new nested node.
							 * All successive chained operations after this
							 * operate on the new node.
							 */
							WriteChain node(const std::string &id)
							{	return WriteChain(
								mNode.createNode(id),this); }
							
							/**
							 * @brief Ends operations on a nested node.
							 * This closes an associated \c node() call.
							 */
							WriteChain &endnode()
							{	return *mParent; }
							
							/**
							 * @brief Creates a node and its first argument.
							 * This is a convenience function that merges
							 * a call to node().arg().endnode().
							 */
							template<class T>
							WriteChain &nodearg(const std::string &id,
								const T &arg=T())
							{	return node(id).arg(arg).endnode(); }
							
							/// @name Specialization tools
							//@{
								/// Creates a new argument for the node
								std::string &createarg()
								{	return mNode.createArg(); }
							//@}
							
						private:
							Node &mNode;
							WriteChain *mParent;
					};
					
					/**
					 * @brief Contains a read-chain.
					 * Allows reading from a node using chained functions.
					 */
					class ReadChain {
						public:
							ReadChain(const Node &node,
								ReadChain *parent=0) :
								mNode(node), mArgIndex(0), mParent(parent) {}
							
							/**
							 * @brief Template function for chained reading.
							 * This may be specialized to implement readers
							 * for other types, the default implementation
							 * catches any type that provides an assignment
							 * operator to assign to \c std::string.
							 */
							template<class T>
							void readArg(T &dst)
							{	dst = nextarg(); }
							
							/**
							 * @brief Reads an argument from the current node.
							 * The result is stored in \p val.
							 * 
							 * @see readArg
							 */
							template<class T>
							ReadChain &arg(T &val)
							{ readArg(val); return *this; }
							
							/// Returns a node chain for a child node.
							ReadChain node(const std::string &id)
							{	return ReadChain(
								mNode.getNode(mNode.indexOf(id)),this); }
							
							/**
							 * @brief Ends operations on a child node,
							 * This closes an associated \c node() call.
							 */
							ReadChain &endnode()
							{	return *mParent; }
							
							/**
							 * @brief Reads a node and its first argument.
							 * This is a convenience function to read a node
							 * and its first argument directly, without having
							 * to call node().arg().endnode().
							 */
							template<class T>
							ReadChain &nodearg(const std::string &id,
								T &v)
							{	return node(id).arg(v).endnode(); }
							
							/// @name Specialization tools
							//@{
								/// Reads the next node argument.
								const std::string &nextarg()
								{	return mNode.getArg(mArgIndex++); }
								
								/// Gets the number of arguments for the node.
								size_t argcount()
								{	return mNode.getArgCount(); }
							//@}
							
						private:
							const Node &mNode;
							size_t mArgIndex;
							ReadChain *mParent;
					};
					
					/// Constructs a write chain for the current node.
					WriteChain writeChain()
					{ return WriteChain(*this); }
					
					/// Constructs a read chain for the current node.
					ReadChain readChain() const
					{ return ReadChain(*this); }
				//@}
				
				/// @name Output
				//@{
					void makeString(std::ostringstream &str,
						bool relRef, int ind
					);
					void makeArgString(std::ostringstream& str,
						const std::string& arg, int ind
					);
				//@}
				
			private:
				size_t hasCircularReferenceInt(ConstNodeSet &nset) const;
				
				std::string mId;	///< Node identifier.
				uint32_t mLine;
				uint32_t mCol;
				Node *mParent;		///< Parent node.
				Nepeta *mHost;	///< Script owning this node.
				
				NodeVector mNodes;	///< Nested nodes for this node.
				ArgVector mArgs;	///< String arguments for this node.
				
				NodeVector mRefs;	///< References to other nodes.
				NodeSet mHasRef;	///< All nodes refererencing this node.
				
				static const std::string sEmpty;	///< "empty" string.
				
				friend class Nepeta;
		};
		
		/**
		 * @brief Enumeration for all possible error types.
		 */
		enum ErrorType {
			/// An illegal character occurred.
			ErrIllegalCharacter = 0,
			
			/// The script came to an abrupt end without closing the node
			ErrPrematureEnd,
			
			/// A comment was opened but never closed
			ErrCommentNotClosed,
			
			/// A string was opened but never closed. 
			ErrNoCloseString,
			
			/// An argument block was opened but never closed.
			ErrBlockNoClosing,
			
			/// A circular reference was detected and ignored
			ErrCircularRef,
			
			/// An invalid reference was detected and ignored
			ErrInvalidRef,
			
			/// A newline was required at the given position, the extra
			/// data was ignored.
			WarnReqNewline,
			
			/// The starting value for warnings.
			WarningStart = WarnReqNewline
		};
		
		/// Contains information about a compilation error.
		struct Error {
			Error(ErrorType type) : type(type), msg(""),
				stLine(0), stCol(0), enLine(0), enCol(0)
				{}
			
			ErrorType type;			///< Error type
			std::string msg;		///< Additional message
			size_t stLine,stCol;	///< Starting line/col for the error
			size_t enLine,enCol;	///< Ending line/col for the error
		};
		typedef std::vector<Error> ErrorVec;
		
		void compile(const std::string &str);
		const ErrorVec &getError() const
		{	return mErrors; }
		
		static std::string errorStr(const Error &err);
		std::string fullErrorStr();
		
		std::string makeString(bool useRelRef=true);
		
		/// Clears all data in this Nepeta object
		void clear() {
			mRoot.clear();
			mErrors.clear();
		}
		
		/// Returns the root node
		Node &getRoot()
		{	return mRoot; }
		
		/// Returns the empty node, clearing it if data was written to it.
		Node &getEmpty()
		{	mEmpty.clear(); return mEmpty; }
		
		/// Inserts a new compilation error
		void addError(ErrorType type, const std::string &msg,
			size_t sL, size_t sC, size_t eL=0, size_t eC=0)
		{
			Error err = Error(type);
			err.msg = msg;
			err.stLine = sL;
			err.stCol = sC;
			err.enLine = eL;
			err.enCol = eC;
			mErrors.push_back( err );
		}
		
		Nepeta() :
			mRoot(this), mEmpty(this), mErrors() {}
		~Nepeta() {}
		
	private:
		Node mRoot;
		Node mEmpty;
		ErrorVec mErrors;
		
		friend class Node;
};

#define _NEPETA__DETAIL__
#include "detail/nepnode.h"
#include "detail/nepreference.h"
#include "detail/nepiterator.h"
#undef _NEPETA__DETAIL__

#endif
