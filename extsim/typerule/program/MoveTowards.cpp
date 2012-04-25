#include "../../ExtSim.h"
#include "MoveTowards.h"

#include "../../param/Position.h"

namespace ExtS { namespace Prog {
	MoveTowardsRule::MoveTowardsRule()
	{
		registerRuleParam(new PositionParam("Position"));
	}
	
	Sim::IdType MoveTowardsRule::registerSimData(ExtSim& esim,
		const std::string &name)
	{
		return esim.getSim().getData().getProgramDb().
			registerImpl<Sim::Prog::MoveTowards>(name);
	}

	
	void MoveTowardsRule::makeInput(ExtSim& extsim,
		Sim::IdType simTypeId, const ParamList* param) const
	{
		Sim::Vector pos = param->getParam<PositionParam>(0)->getVal();
		
		extsim.getSim().getInput().getProgramInput().
		buildInputImpl<Sim::Prog::MoveTowards>(
			Sim::Prog::MoveTowards::Config(
				Sim::Prog::MoveTowards::DtPosition,
				pos
			),
			simTypeId
		);
	}
} }
