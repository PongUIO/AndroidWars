#include "ReplayManager.h"

namespace exts {
	ReplayManager::ReplayManager(ExtSim& esim) :
		mExtSim(esim), mTree(), mActiveNode(0),
		mPhaseSaveInterval(-1)
	{}
	
	ReplayManager::~ReplayManager() {}
	
	ReplayNode* ReplayManager::getActiveNode()
	{
		return 0;
	}
	
	/**
	 * @brief Switches the active branch node
	 */
	void ReplayManager::selectBranch(Sim::IdType id)
	{
		mActiveNode = id;
	}
	
	void ReplayManager::commitNewBranch()
	{
		ReplayNode *node = getActiveNode();
		commit();
	}
	
	void ReplayManager::commit()
	{
		
	}

}
