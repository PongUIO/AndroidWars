#include "../../ExtSim.h"
#include "MoveTowards.h"

#include "../../param/Position.h"

namespace ExtS { namespace Prog {
	MoveTowardsRule::MoveTowardsRule()
	{
		registerRuleParam(new PositionParam("Position"));
	}
	
	void MoveTowardsRule::makeInput(
		ExtSim& extsim, const ParamList* param) const
	{
		Sim::Vector pos = param->getParam<PositionParam>(0)->getVal();
		
		extsim.getSim().getInput().getProgramInput().
		buildInputImpl<Sim::Prog::MoveTowards>(
			Sim::Prog::MoveTowards::Config(
				Sim::Prog::MoveTowards::DtPosition,
				pos
			)
		);
	}
} }
