#include "ExtAbility.h"

#include "../ExtSim.h"

namespace exts {
	// ExtAbilityData
	// 
	// 
	ExtAbilityData::ExtAbilityData(ExtSim& esim) :
		DefaultExtData<ExtAbility>(esim)
	{
	}

	ExtAbilityData::~ExtAbilityData()
	{
		
	}
	
	// ExtAbility
	// 
	// 
	ExtAbility::ExtAbility(ExtSim *esim) : ExtBaseDataObj(esim)
	{}

	ExtAbility::~ExtAbility()
	{
		if(mRule)
			delete mRule;
	}
	
	void ExtAbility::loadNode(Nepeta::Node& node, TypeRule* rule)
	{
		node.readChain().
			nodearg("Name",mName).
			nodearg("Description",mDescription)
		;
		if(mRule)
			delete mRule;
		mRule = rule;
	}
	
	void ExtAbility::postProcess(ExtSim& extsim)
	{
		mRule->postProcess(extsim);
	}
}
