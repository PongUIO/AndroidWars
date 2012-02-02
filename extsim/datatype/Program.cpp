#include "Program.h"

#include "../ExtSim.h"

#include "../typerule/program/ArmorAttachment.h"

namespace ExtS {
	// ProgramData
	//
	//
	ProgramData::ProgramData(ExtSim &esim) :
		DefaultData<ExtProgram>(esim)
	{
		registerTypeRule("Ability/ArmorAttachment",
			new Prog::ArmorAttachmentRule());
	}
	
	ProgramData::~ProgramData()
	{
		
	}

	void ProgramData::loadBlock(Script::Block& block)
	{
		const std::string &name = block.getDataFirst("Name");
		if(name.empty())
			return;
		
		TypeRule *rule = loadRuleBlock(block);
		if(rule) {
			ExtProgram *extProgram = createType();
			
			extProgram->loadBlock(block, rule);
		} else printf("Program '%s' does not inherit any valid rule\n",
			name.c_str());
	}

	void ProgramData::postProcess()
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
	{}

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
