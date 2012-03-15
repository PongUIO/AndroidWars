#include "ExtProgram.h"

#include "../ExtSim.h"

#include "../typerule/program/ArmorAttachment.h"
#include "../typerule/program/MoveTowards.h"

namespace ExtS {
	// ExtProgramData
	//
	//
	ExtProgramData::ExtProgramData(ExtSim &esim) :
		DefaultExtData<ExtProgram>(esim)
	{
		registerTypeRule("Ability/ArmorAttachment",
			new Prog::ArmorAttachmentRule());
		registerTypeRule("Base/MoveTowards",
			new Prog::MoveTowardsRule());
	}
	
	ExtProgramData::~ExtProgramData()
	{
		
	}

	void ExtProgramData::loadBlock(Script::Block& block)
	{
		const std::string &name = block.getDataFirst("Name");
		if(name.empty())
			return;
		
		TypeRule *rule = loadRuleBlock(block);
		if(rule) {
			ExtProgram *extProgram = createType();
			mNameIdMgr.connect(extProgram->getId(), name);
			
			extProgram->loadBlock(block, rule);
		} else printf("Program '%s' does not inherit any valid rule\n",
			name.c_str());
	}

	void ExtProgramData::postProcess()
	{
		for(DataVec::iterator i=mData.begin(); i!=mData.end(); ++i) {
			(*i)->postProcess(mExtSim);
		}
	}
	
	// ExtProgram
	//
	//
	ExtProgram::ExtProgram() : mName(), mRule(0)
	{}
	
	ExtProgram::~ExtProgram()
	{
		if(mRule)
			delete mRule;
	}

	void ExtProgram::loadBlock(Script::Block& block, TypeRule *rule)
	{
		mName = block.getDataFirst("Name");
		mDescription = block.getDataFirst("Description");
		mRule = rule;
	}

	void ExtProgram::postProcess(ExtSim& extsim)
	{
		mRule->postProcess(extsim);
	}

}
