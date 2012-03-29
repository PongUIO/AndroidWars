#include "ExtBaseData.h"

namespace ExtS {
	// ExtBaseData
	// 
	// 
	void ExtBaseDataObj::loadBlock(Script::Block& block, TypeRule *rule)
	{
		mName = block.getDataFirst("Name");
		mDescription = block.getDataFirst("Description");
		
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
