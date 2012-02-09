#include "Game.h"

#include "../ExtData.h"
#include "../ExtSim.h"

namespace ExtS {
	// GameData
	//
	//
	GameData::GameData(ExtSim& sim): BaseData(sim)
	{}
	
	GameData::~GameData()
	{}
	
	void GameData::startup()
	{}
	
	void GameData::shutdown()
	{}
	
	void GameData::loadBlock(Script::Block& block)
	{
		mName = block.getDataFirst("Name");
		mDescription = block.getDataFirst("Description");
		
		mStepTime = ExtData::readValue<double>(
			block.getDataFirst("StepTime"), 1.0);
	}

	void GameData::postProcess()
	{
		mExtSim.getSim().getConfig().stepTime = mStepTime;
	}

}
