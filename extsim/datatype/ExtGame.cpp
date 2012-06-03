#include "ExtGame.h"

#include "../ExtData.h"
#include "../ExtSim.h"

namespace ExtS {
	// ExtGameData
	//
	//
	ExtGameData::ExtGameData(ExtSim& sim): ExtBaseData(sim)
	{}
	
	ExtGameData::~ExtGameData()
	{}
	
	void ExtGameData::startup()
	{}
	
	void ExtGameData::shutdown()
	{}
	
	void ExtGameData::loadNode(Nepeta::Node& node)
	{
		node.readChain().
			nodearg("Name", mName).
			nodearg("Description", mDescription).
			nodearg("StepTime", mStepTime);
	}

	void ExtGameData::postProcess()
	{
		mExtSim->getSim().getConfiguration().stepTime = mStepTime;
	}

}
