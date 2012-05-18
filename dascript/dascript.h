#ifndef SCRIPT_H
#define SCRIPT_H

#include <string>
#include <set>
#include <vector>
#include <map>

#include <stdint.h>

class DaCodec {
	public:
		typedef std::map<std::string, DaCodec*> CodecPack;
		
		DaCodec() {}
		virtual ~DaCodec() {}
		
		virtual void encode(std::string &str)=0;
		virtual void decode(std::string &str)=0;
		
	private:
};

class DaScript {
	public:
		/**
		 * Node object.
		 * Contains an identifier, an argument list, a list of nested nodes,
		 * and a list of references.
		 */
		class Node {
			private:
				typedef std::vector<Node*> NodeVector;
				typedef std::set<const Node*> ConstNodeSet;
				typedef std::set<Node*> NodeSet;
				typedef std::vector<std::string> ArgVector;
				
				Node(DaScript *src, Node *parent=0, const std::string &id="",
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
				
				Node &operator=(const DaScript::Node &other) {
					if(this != &other) {
						// Remove everything in this node
						clear();
						
						// Copy metadata
						mId = other.mId;
						mLine = other.mLine;
						mCol = other.mCol;
						mParent = other.mParent;
						mHost = other.mHost;
						
						// Copy nodes
						for(NodeVector::const_iterator i=other.mNodes.begin();
							i!=other.mNodes.end(); ++i) {
							mNodes.push_back(new Node(**i));
						}
						
						// Copy arguments
						mArgs = other.mArgs;
						
						// Note: References are not copied, as it is
						// not possible to trivially detect the reference
						// mappings. Rebuilding references must be done
						// manually.
					}
					
					return *this;
				}
				
			public:
				/// @name System
				/// Manages special functions related for the
				/// node system.
				//@{
					static const size_t NoPos;
					
					/**
					 * Returns a reference to the special "empty" node.
					 * This node always hold the same pointer for a script object.
					 *
					 * This node has the following important properties:
					 * - The empty node, while possible to modify, always
					 * 	resets its value every time this is called.
					 * - The node may be modified, but no modifications are
					 * 	changed long-term.
					 */
					Node &getEmpty() const
					{	return mHost->getEmpty(); }
					
					/**
					 * Erases all data in the node recursively.
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
					 * Returns \c true if this node is a "valid" node.
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
					size_t getArgCount() const { return mArgs.size(); }
					
					/**
					 * Creates a new argument and appends it to the end of
					 * the argument list.
					 */
					std::string &createArg(const std::string &str="")
					{	setArg(getArgCount(),str); return mArgs.back(); }
					
					/**
					 * Sets the argument string at the given index.
					 * The argument list is resized to fit.
					 */
					void setArg(size_t index=0, const std::string &str="") {
						if(index>=getArgCount())
							mArgs.resize(index+1);
						mArgs[index] = str;
					}
					
					/**
					 * Returns the argument for this node at the given index.
					 * Out-of-bounds arguments return an empty string.
					 */
					const std::string &getArg(size_t index=0) const
					{ return (index<getArgCount()) ? mArgs[index] : sEmpty; }
					
					/**
					 * Removes an argument, shifting
					 * the argument list as needed.
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
					 * Contains a write-chain.
					 * 
					 * A write chain is a a chain of functions allowing
					 * recursive node and argument building in a simple manner.
					 */
					class NodeWriteChain {
						public:
							NodeWriteChain(Node &node,
								NodeWriteChain *parent=0) :
								mNode(node), mParent(parent) {}
							
							/**
							 * Creates and appends a new argument with the
							 * given value for the active node.
							 */
							NodeWriteChain &arg(const std::string &str="")
							{	mNode.createArg(str); return *this; }
							
							/**
							 * Creates a new nested node for the active node.
							 * All successive chained operations after this
							 * operate on the new node.
							 */
							NodeWriteChain node(const std::string &id)
							{	return NodeWriteChain(
								mNode.createNode(id),this); }
							
							/**
							 * Ends operations on a nested node, closing
							 * an associated \c node() call.
							 */
							NodeWriteChain &endnode()
							{	return *mParent; }
							
						private:
							Node &mNode;
							NodeWriteChain *mParent;
					};
					
					/**
					 * Contains a read-chain.
					 * 
					 * Allows reading from a node using chained functions.
					 */
					class NodeReadChain {
						public:
							NodeReadChain(const Node &node,
								NodeReadChain *parent=0) :
								mNode(node), mArgIndex(0), mParent(parent) {}
							
							/**
							 * May be specialized to provide argument readers
							 * for other types, the default implementation
							 * catches any type that provides an assignment
							 * operator to assign to strings.
							 */
							template<class T>
							void readArg(T &dst, const std::string &str)
							{	dst = str; }
							
							/**
							 * Read an argument using a template function.
							 * 
							 * The argument is stored in the output parameter
							 * \p val.
							 */
							template<class T>
							NodeReadChain &argt(T &val)
							{	readArg(val,mNode.getArg(mArgIndex++));
								return *this; }
							
							/**
							 * Reads an argument from the current chain,
							 * storing the result in \p str. Basically the
							 * "default" version of \c argt().
							 */
							NodeReadChain &arg(std::string &str)
							{ return argt<std::string>(str); }
							
							/**
							 * Returns a node chain for the first node with
							 * the given identifier.
							 */
							NodeReadChain node(const std::string &id)
							{	return NodeReadChain(
								mNode.getNode(mNode.indexOf(id)),this); }
							
							/**
							 * Ends operations on a nested node, closing an
							 * associated \c node() call.
							 */
							NodeReadChain &endnode()
							{	return *mParent; }
							
						private:
							const Node &mNode;
							size_t mArgIndex;
							NodeReadChain *mParent;
					};
					
					/// Constructs a write chain for the current node.
					NodeWriteChain writeChain()
					{ return NodeWriteChain(*this); }
					
					/// Constructs a read chain for the current node.
					NodeReadChain readChain() const
					{ return NodeReadChain(*this); }
				//@}
				
			private:
				size_t hasCircularReferenceInt(ConstNodeSet &nset) const;
				
				std::string mId;	///< Node identifier.
				uint32_t mLine;
				uint32_t mCol;
				Node *mParent;		///< Parent node.
				DaScript *mHost;	///< Script owning this node.
				
				NodeVector mNodes;	///< Nested nodes for this node.
				ArgVector mArgs;	///< String arguments for this node.
				
				NodeVector mRefs;	///< References to other nodes.
				NodeSet mHasRef;	///< All nodes refererencing this node.
				
				static const std::string sEmpty;	///< "empty" string.
				
				friend class DaScript;
		};
		
		/**
		 * Enumeration for possible error types.
		 */
		enum ErrorType {
			/// A string was opened but never closed. 
			ErrNoCloseString = 0,
			
			/// An argument block was opened but never closed.
			ErrBlockNoClosing,
			
			/// An illegal character occurred.
			ErrIllegalCharacter,
			
			/// The script came to an abrupt end without closing the node
			ErrPrematureEnd,
			
			/// A circular reference was detected and ignored
			ErrCircularRef,
			
			/// An invalid reference was detected and ignored
			ErrInvalidRef,
			
			/// A newline was required at the given position, the extra
			/// data was ignored.
			WarnReqNewline,
			
			WarningStart = WarnReqNewline
		};
		
		/**
		 * Contains information about errors
		 * that occured during compilation.
		 */
		struct Error {
			Error(ErrorType type) : type(type), msg(""),
				stLine(0), stCol(0), enLine(0), enCol(0)
				{}
			
			ErrorType type;
			std::string msg;
			size_t stLine,stCol;
			size_t enLine,enCol;
		};
		typedef std::vector<Error> ErrorVec;
		
		void compile(const std::string &str);
		const ErrorVec &getError() const
		{	return mErrors; }
		
		static std::string errorStr(const Error &err);
		std::string fullErrorStr();
		
		std::string makeString(bool useRelRef=true);
		
		/// Clears all data in this object
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
		
		DaScript() :
			mRoot(this), mEmpty(this), mErrors() {}
		~DaScript() {}
		
	private:
		Node mRoot;
		Node mEmpty;
		ErrorVec mErrors;
		
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
		
		void makeArg(std::stringstream& str, const std::string& arg, int ind);
		void makeNodeString(std::stringstream &str, Node &node, bool relRef,
			int ind);
		
		friend class DaParser;
		friend class Node;
};

#define _DASCRIPT__DETAIL__
#include "detail/danode.h"
#include "detail/dareference.h"
#undef _DASCRIPT__DETAIL__

#endif
