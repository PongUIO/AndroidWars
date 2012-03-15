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
	
	void ExtGameData::loadBlock(Script::Block& block)
	{
		mName = block.getDataFirst("Name");
		mDescription = block.getDataFirst("Description");
		
		mStepTime = ExtData::readValue<double>(
			block.getDataFirst("StepTime"), 1.0);
	}

	void ExtGameData::postProcess()
	{
		mExtSim.getSim().getConfiguration().stepTime = mStepTime;
	}

}
