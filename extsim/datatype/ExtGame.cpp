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
			node("Name").arg(mName).endnode().
			node("Description").arg(mDescription).endnode();
		
		mStepTime = ExtData::readValue<double>(
			node.getNodeFirst("StepTime"), 1.0);
	}

	void ExtGameData::postProcess()
	{
		mExtSim->getSim().getConfiguration().stepTime = mStepTime;
	}

}
