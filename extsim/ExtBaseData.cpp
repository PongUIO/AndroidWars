#include "ExtBaseData.h"

namespace ExtS {
	// ExtBaseData
	// 
	// 
	void ExtBaseDataObj::loadNode(Nepeta::Node& node,
		Sim::IdType simTypeId, TypeRule *rule)
	{
		node.readChain()
			.nodearg("Name", mName)
			.nodearg("Description", mDescription);
		
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
