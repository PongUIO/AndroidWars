#include "ExtProgram.h"

#include "../ExtSim.h"

#include "../typerule/program/program.h"

namespace exts {
	// ExtProgramData
	//
	//
	ExtProgramData::ExtProgramData(ExtSim &esim) :
		DefaultExtData<ExtProgram>(esim), mRuleLoader(esim)
	{
		/*registerTypeRule("Base/MoveTowards",
			new Prog::MoveTowardsRule());*/
#define _EXTS_X(name, cls) mRuleLoader.registerRule(name, new prog::cls(mExtSim));
		#include "../typerule/program/program.def"
#undef _EXTS_X
	}
	
	ExtProgramData::~ExtProgramData()
	{
		
	}
	
	// ExtProgram
	//
	//
	ExtProgram::ExtProgram(ExtSim &esim, Sim::IdType id) :
		ExtDataObjBase(esim,id)
	{}
	
	ExtProgram::~ExtProgram()
	{}

	void ExtProgram::loadNode(const Nepeta::Node& node)
	{
		ExtDataObjBase::loadNode(node);
		
		mRule = mExtSim.getData().getProgramDb().mRuleLoader.loadRuleNode(node);
		if(mRule)
			mRule->setObjectId(getId());
	}
	
	void ExtProgram::postProcess()
	{
		ExtDataObjBase::postProcess();
	}
}
