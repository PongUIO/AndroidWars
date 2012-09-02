#include <list>
#include <assert.h>
#include <stdio.h>

#include "ReplayManager.h"

#include "../ExtSim.h"

namespace exts {
	ReplayManager::ReplayManager(ExtSim& esim) :
		mExtSim(esim), mTree(), mActiveNode(0),
		mPhaseSaveInterval(-1)
	{}
	
	ReplayManager::~ReplayManager() {}
	
	/**
	 * @brief Switches the active branch node
	 * 
	 * The active branch identifies the endpoint for a replay and where the
	 * next data is committed to.
	 */
	void ReplayManager::selectBranch(Sim::IdType id)
	{
		if(!mTree.getNode(id))
			return mExtSim.getError().msgId(EXTS_FUNC, id);
		
		mActiveNode = id;
	}
	
	/**
	 * @brief Replays the active branch to the specified time.
	 * 
	 * This works by looking up the most recent ancestor with a full copy of
	 * the simulation, and replaying everything from that point until the
	 * given phase and step.
	 */
	void ReplayManager::replay(uint32_t phase, uint32_t step)
	{
		Sim::Simulation &sim = mExtSim.getSim();
		Sim::Configuration &simCfg = sim.getConfiguration();
		Sim::State &simState = sim.getState();
		
		// Find the correct phase node, this is the base node for replay
		ReplayNode *baseNode = getActiveNodeI();
		while(baseNode && baseNode->getDepth() > phase)
			baseNode = baseNode->getParent();
		
		// Assure the node exists
		if(!baseNode)
			return;
		
		// Find the most recent complete simulation save, and store the path
		// to it
		ReplayList nodePath;
		const ReplayNode *mostRecentSimC =
		buildPathToSimSave(nodePath, baseNode->getId());
		
		// Minor hack to get around const-correctness issues caused by me.
		ReplayNode *mostRecentSim = mTree.getNode(mostRecentSimC->getId());
		
		// Assure the node exists, if it doesn't there is no parent node
		// with a simulation save
		if(!mostRecentSim)
			return;
		
		// Load this simulation copy
		mExtSim.load(mostRecentSim->getData(ReplayNode::NtSimulation));
		
		// Start replaying to the correct phase and step
		uint32_t totalStep = simCfg.phaseLength*phase + step;
		stepReplay(nodePath, totalStep - sim.getCurTotalStep() );
	}
	
	/**
	 * @brief Builds a path from the given node to the nearest parent with
	 * a complete simulation save.
	 * 
	 * @return The node with the complete simulation save
	 */
	const ReplayNode *ReplayManager::buildPathToSimSave(ReplayList& path, Sim::IdType nodeId)
	{
		ReplayNode *node = mTree.getNode(nodeId);
		if(!node)
			return 0;
		
		while(node && !node->isTypeSet(ReplayNode::NtSimulation)) {
			path.push_front(node);
			node = node->getParent();
		}
		
		return node;
	}
	
	/**
	 * @brief Loads the next input from a replay path
	 * 
	 * The input is loaded to the \c InputBarrier, but is not dispatched.
	 * 
	 * @param src The source path to take from, the front element is discarded
	 * when used.
	 */
	void ReplayManager::loadPhaseInput(ReplayList& src)
	{
		if(src.size() == 0)
			return;
		
		const ReplayNode *curNode = src.front();
		
		// Assure the input can be loaded
		// - Node must exist
		// - The node must have a depth one above the current phase
		// - The simulation state must be outside phase processing (idle)
		Sim::State &simState = mExtSim.getSim().getState();
		if(!curNode ||
		simState.getCurPhase()+1 != curNode->getDepth() ||
		simState.getStateType() != Sim::State::StIdle) {
			return;
		}
		
		// Load the input
		mExtSim.getInput().loadInput(curNode->getData(ReplayNode::NtInput));
		
		// Discard the node
		src.pop_front();
	}
	
	void ReplayManager::replay(double timeUnit)
	{
		const Sim::Configuration &cfg = mExtSim.getSim().getConfiguration();
		double phaseTime = cfg.stepTime*cfg.phaseLength;
		
		uint32_t phase = timeUnit / phaseTime;
		uint32_t step = (fmod(timeUnit, phaseTime)/phaseTime)*cfg.phaseLength;
		
		return replay(phase,step);
	}
	
	/**
	 * @brief Steps the replay one step forwards
	 * 
	 * Everything related to stepping is taken care of by this function, such
	 * as loading input to the \c InputBarrier, and calling \c startPhase and
	 * \c endPhase.
	 * 
	 * @param path A precomputed path storing the successive nodes to retrieve
	 * input from.
	 */
	void ReplayManager::stepReplay(ReplayList &path, uint32_t numStep)
	{
		Sim::Simulation &sim = mExtSim.getSim();
		Sim::State &state = sim.getState();
		
		while( (numStep--) > 0) {
			// Load input
			if( state.getStateType() == Sim::State::StIdle ) {
				loadPhaseInput(path);
				mExtSim.getInput().dispatchInput(false);
				
				sim.startPhase();
			}
			
			// Step
			if( state.getStateType() == Sim::State::StInPhase )
				sim.step();
			
			if( state.getStateType() == Sim::State::StEndPhase )
				sim.endPhase();
		}
	}
	
	/**
	 * @brief Loads the simulation state at the active node
	 */
	void ReplayManager::gotoActive()
	{
		ReplayNode *node = getActiveNodeI();
		
		if(node) {
			replay(node->getDepth(),0);
		}
	}

	
	/**
	 * @brief Appends a new node to the active node and commits to it.
	 */
	void ReplayManager::commitNewBranch()
	{
		ReplayNode *node = getActiveNodeI();
		if(node) {
			ReplayNode *branch = node->createBranch();
			selectBranch(branch->getId());
			commit();
		}
	}
	
	/**
	 * @brief Commits the active state to the \c ReplayTree
	 * 
	 * This stores, depending on the \c ReplayManager settings, various data
	 * for the simulation and extended simulation.
	 */
	void ReplayManager::commit()
	{
		uint32_t activePhase = mExtSim.getSim().getCurPhase();
		
		// Assure the simulation is in a proper state
		if(mExtSim.getSim().getState().getStateType() != Sim::State::StIdle) {
			return mExtSim.getError().message(exts::ExtErrorMgr::Warning,
				EXTS_FUNC, "The simulation state is not in the idle state"
			);
		}
		
		// Find a suitable node to commit to
		ReplayNode *node = findNodeForPhase(activePhase);
		if(!node) {
			return mExtSim.getError().message(
				exts::ExtErrorMgr::Warning, EXTS_FUNC,
				"Could not find a suitable node for the current branch "
				"and phase"
			);
		}
		
		// Destroy the node's subtree
		node->modifyNode();
		if(getActiveNodeI() == 0)
			selectBranch(node->getId());
		
		// Save the simulation
		if(node->getDepth()%mPhaseSaveInterval == 0) {
			mExtSim.save(node->getData(ReplayNode::NtSimulation));
		}
		
		// Save the InputBarrier
		Sim::Save::FilePtr fp(node->getData(ReplayNode::NtInput));
		mExtSim.getInput().saveInput(fp);
	}
	
	/**
	 * @brief Seeks in the current branch for a node at the phase.
	 */
	ReplayNode* ReplayManager::findNodeForPhase(uint32_t phase)
	{
		// Look backwards from the last node
		ReplayNode *node = getActiveNodeI();
		
		while(node) {
			if(node->getDepth() == phase)
				return node;
			
			node = node->getParent();
		}
		
		return node;
	}
}
