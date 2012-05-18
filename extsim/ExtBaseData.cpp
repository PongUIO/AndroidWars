#include "ExtBaseData.h"

namespace ExtS {
	// ExtBaseData
	// 
	// 
	void ExtBaseDataObj::loadNode(DaScript::Node& node,
		Sim::IdType simTypeId, TypeRule *rule)
	{
		mName = node.getNodeFirst("Name");
		mDescription = node.getNodeFirst("Description");
		
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
