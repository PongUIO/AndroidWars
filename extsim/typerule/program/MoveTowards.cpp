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
		Sim::Prog::MoveTowards::Config cfg;
		cfg.mTargetPos = param->getParamT<PositionParam>(0)->getVal();
		
		mExtSim.getSim().getInput().getProgramInput().
		buildInputImpl<Sim::Prog::MoveTowards>(cfg, mObjectId);
	}
} }
