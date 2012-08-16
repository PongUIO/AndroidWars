#include "ReplayManager.h"

namespace exts {
	ReplayManager::ReplayManager(ExtSim& esim) :
		mPhaseSaveInterval(-1), mDoBranchSave(true),
		mTree(), mExtSim(&esim)
	{}
	
	ReplayManager::~ReplayManager() {}
}
