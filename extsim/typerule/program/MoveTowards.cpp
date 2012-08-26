#include <stdio.h>

#include "MoveTowards.h"

#include "../../ExtSim.h"

#include "../../param/Position.h"
#include "../../param/ValRange.h"

namespace exts { namespace prog {
	MoveTowards::MoveTowards(ExtSim& esim): TypeRule(esim)
	{
		registerParam(new PositionParam("Position"));
		registerParam(new ValRange<uint32_t>("Duration"));
	}
	
	MoveTowards::~MoveTowards() {}
	
	Sim::IdType MoveTowards::registerSimData(const std::string& name) const
	{
		return mExtSim.getSim().getData().getProgramDb().
		registerImpl<Sim::Prog::MoveTowards>(name);
	}
	
	void MoveTowards::makeInput(const ParamList* param) const
	{
		const PositionParam *pos = param->getParamT<PositionParam>(0);
		const ValRange<uint32_t> *duration = param->getParamT<ValRange<uint32_t> >(1);
		
		Sim::Prog::MoveTowards::Config cfg = Sim::Prog::MoveTowards::Config(
			Sim::Prog::MoveTowards::DtPosition,pos->getVal()
		);
		
		Sim::Prog::MoveTowards *mt = mExtSim.getSim().getInput().getProgramInput().
		buildInputImpl<Sim::Prog::MoveTowards>(cfg, mObjectId);
		
		mt->setRunningTime(duration->getVal());
	}
} }
