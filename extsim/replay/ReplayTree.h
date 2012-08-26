#ifndef EXTSIM_REPLAYTREE_H
#define EXTSIM_REPLAYTREE_H

#include <vector>
#include <stack>

#include "../simulation/Save.h"
#include "../simulation/Common.h"

namespace exts {
	class ReplayTree;
	class ReplayNode;
	
	typedef std::vector<ReplayNode*> ReplayNodeVec;
	
	/**
	 * @brief Replay tree node that contains all relevant data for a phase.
	 * 
	 * A node contains one or more \c Save objects for different aspects
	 * related to \c Simulation replays, including metadata such as the
	 * timeline used to generate the input, or full simulation state saves.
	 */
	class ReplayNode {
		public:
			/// Identifies the types of data stored in this node
			enum NodeType {
				/// Copy of the simulation state
				NtSimulation	= 0,
				/// Iterative save of raw input
				NtInput			= 1,
				/// Save of the \c TimelineManager for this phase
				NtTimeline		= 2,
				
				NtMax
			};
			
			const Sim::Save &getData(NodeType type) const
			{ return mSave[type]; }
			Sim::Save &getData(NodeType type) { return mSave[type]; }
			
			bool isTypeSet(NodeType type) const
			{ return mSave[type].size()>0; }
			
			bool isRoot() const { return !mParent; }
			bool isBranch() const { return mChildren.size()>0; }
			bool isLeaf() const { return mChildren.size()==0; }
			
			ReplayNode *getParent() { return mParent; }
			const ReplayNode *getParent() const { return mParent; }
			
			ReplayNodeVec &getChildren() { return mChildren; }
			const ReplayNodeVec &getChildren() const { return mChildren; }
			
			size_t getChildrenCount() const { return mChildren.size(); }
			size_t getDepth() const { return mDepth; }
			
			Sim::IdType getId() const { return mId; }
			
			ReplayNode *createBranch();
			void modifyNode();
			
		private:
			ReplayNode(ReplayTree &mgr, Sim::IdType id, ReplayNode *parent=0);
			~ReplayNode();
			
			ReplayTree &mTree;
			Sim::IdType mId;
			
			ReplayNode *mParent;
			ReplayNodeVec mChildren;
			size_t mDepth;
			
			Sim::Save mSave[NtMax];
			
			friend class ReplayTree;
	};
	
	/**
	 * @brief Holds a branching tree of alternate timelines for phases.
	 * 
	 * The \c ReplayTree has the role of keeping distinct timelines organized
	 * in such a way that \c ExtSim can simulate several branching timelines
	 * with a common ancestry.
	 * 
	 * This is implemented by assuming each branch contain all phase-data up
	 * until its local present. Phase-data for phases before the branch
	 * branched off are transparently available as if the branch was a
	 * linear timeline.
	 */
	class ReplayTree {
		public:
			ReplayTree();
			~ReplayTree();
			
			ReplayNode *getRoot() { return mRoot; }
			const ReplayNode *getRoot() const { return mRoot; }
			
			ReplayNode *getNode(Sim::IdType id)
			{ return id>=mNodes.size() ? 0 : mNodes[id]; }
			const ReplayNode *getNode(Sim::IdType id) const
			{ return const_cast<const ReplayNode*>(getNode(id)); }
			
		private:
			ReplayNode *makeNode(ReplayNode *parent=0);
			void freeNode(ReplayNode *node);
			
			ReplayNode *mRoot;
			
			typedef std::stack<Sim::IdType> FreeStack;
			ReplayNodeVec mNodes;
			FreeStack mFreeNodes;
			
			friend class ReplayNode;
	};
}

#endif
