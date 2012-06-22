#include "ReplayManager.h"

namespace ExtS {
	ReplayManager::ReplayManager(ExtSim& esim) :
		mPhaseSaveInterval(-1), mDoBranchSave(true),
		mTree(), mExtSim(&esim)
	{}
	
	ReplayManager::~ReplayManager() {}
}
