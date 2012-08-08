#include "ExtProgram.h"

#include "../ExtSim.h"

namespace exts {
	// ExtProgramData
	//
	//
	ExtProgramData::ExtProgramData(ExtSim &esim) :
		DefaultExtData<ExtProgram>(esim)
	{
		/*registerTypeRule("Base/MoveTowards",
			new Prog::MoveTowardsRule());*/
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
	}
	
	void ExtProgram::postProcess()
	{
		ExtDataObjBase::postProcess();
	}
}
