#include "MoveTowards.h"

#include "../../ExtSim.h"

#include "../../param/Position.h"

namespace exts { namespace prog {
	MoveTowards::MoveTowards(ExtSim& esim): TypeRule(esim)
	{
		registerParam(new PositionParam("Position"));
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
		
		
		Sim::Prog::MoveTowards::Config cfg = Sim::Prog::MoveTowards::Config(
			Sim::Prog::MoveTowards::DtPosition,pos->getVal()
		);
		
		mExtSim.getSim().getInput().getProgramInput().
		buildInputImpl<Sim::Prog::MoveTowards>(cfg, mObjectId);
	}
} }
