#include <list>
#include <assert.h>

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
		ReplayNode *baseNode = getActiveNode();
		while(baseNode && baseNode->getDepth() > phase)
			baseNode = baseNode->getParent();
		
		// Assure the node exists
		if(!baseNode)
			return;
		
		// Find the most recent complete simulation save, and store the path
		// to it
		typedef std::list<ReplayNode*> ReplayList;
		ReplayList nodePath;
		ReplayNode *mostRecentSim = baseNode;
		nodePath.push_front(mostRecentSim);
		while(mostRecentSim
			&& !mostRecentSim->isTypeSet(ReplayNode::NtSimulation)) {
			mostRecentSim = mostRecentSim->getParent();
			nodePath.push_front(mostRecentSim);
		}
		
		// Assure the node exists, if it doesn't there is no parent node
		// with a simulation save
		if(!mostRecentSim)
			return;
		
		// Load this simulation copy
		sim.load(mostRecentSim->getData(ReplayNode::NtSimulation));
		
		// Start replaying to the correct phase and step
		uint32_t totalStep = simCfg.phaseLength*phase + step;
		ReplayList::iterator i = nodePath.begin();
		while( sim.getCurTotalStep() < totalStep ) {
			
			
			// Load input
			if( simState.getStateType() == Sim::State::StIdle ) {
				ReplayNode *curNode = (i==nodePath.end()) ? 0 : *i;
				
				
				// Dispatch input
				if(curNode) {
					mExtSim.getInput().load(
						curNode->getData(ReplayNode::NtInput)
					);
					mExtSim.getInput().dispatchInput();
					
					++i;
				}
				
				sim.startPhase();
			}
			
			// Step 
			if( simState.getStateType() == Sim::State::StInPhase )
				sim.step();
			
			// End phase
			if( simState.getStateType() == Sim::State::StEndPhase )
				sim.endPhase();
		}
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
	 */
	void ReplayManager::step(uint32_t numStep)
	{
		
	}
	
	/**
	 * @brief Appends a new node to the active node and commits to it.
	 */
	void ReplayManager::commitNewBranch()
	{
		ReplayNode *node = getActiveNode();
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
		ReplayNode *node = getActiveNode();
		
		if(node) {
			// A commit may only be performed when the simulation phase
			// is the same as the node's depth
			assert(mExtSim.getSim().getCurPhase() == node->getDepth() &&
				"The node committed must be in the same phase as the simulation"
			);
			
			// Save the simulation
			if(node->getDepth()%mPhaseSaveInterval == 0) {
				node->getData(ReplayNode::NtSimulation) = 
				mExtSim.getSim().save();
			}
			
			// Save the InputBarrier
			Sim::Save &saveObj = node->getData(ReplayNode::NtInput);
			saveObj.clear();
			Sim::Save::FilePtr fp(saveObj);
			mExtSim.getSim().getInput().save(fp);
		}
	}
}
