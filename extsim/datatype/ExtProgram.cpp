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
	
	// ExtProgram
	//
	//
	ExtProgram::ExtProgram()
	{}
	
	ExtProgram::~ExtProgram()
	{}

	void ExtProgram::loadBlock(Script::Block& block, TypeRule* rule)
	{
		ExtBaseDataObj::loadBlock(block, rule);
	}
	
	void ExtProgram::postProcess(ExtSim& extsim)
	{
		ExtBaseDataObj::postProcess(extsim);
	}
}
