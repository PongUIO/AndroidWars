#include "ExtBaseData.h"

namespace ExtS {
	// ExtBaseData
	// 
	// 
	void ExtBaseDataObj::loadBlock(Script::Block& block,
		Sim::IdType simTypeId, TypeRule *rule)
	{
		mName = block.getDataFirst("Name");
		mDescription = block.getDataFirst("Description");
		
		mId = simTypeId;
		
		if(rule) {
			if(mRule)
				delete mRule;
			
			mRule = rule;
		}
	}
	
	void ExtBaseDataObj::postProcess(ExtSim& extsim)
	{
		if(mRule)
			mRule->postProcess(extsim);
	}
}
