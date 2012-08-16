#include "ExtGame.h"

#include "../util/TypeConv.h"
#include "../CommonLoad.h"
#include "../ExtSim.h"

namespace exts {
	// ExtGameData
	//
	//
	ExtGameData::ExtGameData(ExtSim& esim) : ExtDataComponent(), mExtSim(esim)
	{}
	
	ExtGameData::~ExtGameData()
	{}
	
	void ExtGameData::startup()
	{}
	
	void ExtGameData::shutdown()
	{}
	
	void ExtGameData::loadNode(const Nepeta::Node& node)
	{
		node.readChain().
			nodearg(Load::Name, mName).
			nodearg(Load::Description, mDescription);
		mStepTime = convValue(node.getNodeFirst("StepTime"), 1.0);
	}

	void ExtGameData::postProcess()
	{
		mExtSim.getSim().getConfiguration().stepTime = mStepTime;
	}
}
