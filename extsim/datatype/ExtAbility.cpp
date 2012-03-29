#include "ExtAbility.h"

#include "../ExtSim.h"

namespace ExtS {
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
	ExtAbility::ExtAbility() : mName(), mRule(0)
	{}

	ExtAbility::~ExtAbility()
	{
		if(mRule)
			delete mRule;
	}
	
	void ExtAbility::loadBlock(Script::Block& block, TypeRule* rule)
	{
		mName = block.getDataFirst("Name");
		mDescription = block.getDataFirst("Description");
		if(mRule)
			delete mRule;
		mRule = rule;
	}
	
	void ExtAbility::postProcess(ExtSim& extsim)
	{
		mRule->postProcess(extsim);
	}
}
