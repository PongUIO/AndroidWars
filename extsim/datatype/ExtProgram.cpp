#include "ExtProgram.h"

#include "../ExtSim.h"

#include "../typerule/program/MoveTowards.h"

namespace ExtS {
	// ExtProgramData
	//
	//
	ExtProgramData::ExtProgramData(ExtSim &esim) :
		DefaultExtData<ExtProgram>(esim)
	{
		registerTypeRule("Base/MoveTowards",
			new Prog::MoveTowardsRule());
	}
	
	ExtProgramData::~ExtProgramData()
	{
		
	}
	
	// ExtProgram
	//
	//
	ExtProgram::ExtProgram(ExtSim *esim) : ExtBaseDataObj(esim)
	{}
	
	ExtProgram::~ExtProgram()
	{}

	void ExtProgram::loadNode(Nepeta::Node& node,
		Sim::IdType simTypeId, TypeRule* rule)
	{
		ExtBaseDataObj::loadNode(node, simTypeId, rule);
	}
	
	void ExtProgram::postProcess(ExtSim& extsim)
	{
		ExtBaseDataObj::postProcess(extsim);
	}
}
