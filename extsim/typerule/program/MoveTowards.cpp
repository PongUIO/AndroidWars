#include "../../ExtSim.h"
#include "MoveTowards.h"

#include "../../param/Position.h"

namespace ExtS { namespace Prog {
	MoveTowardsRule::MoveTowardsRule()
	{
		registerRuleParam(new PositionParam("Position"));
	}
	
	bool MoveTowardsRule::makeInput(
		Sim::Save::BasePtr& fp, const ParamList* param) const
	{
		Sim::Vector pos = param->getParam<PositionParam>(0)->getVal();
		
		fp << mHostId;
		mExtSim->getSim().getState().getProgramFactory().
			createAndSerializeType<Sim::Prog::MoveTowards>(
				fp,
				Sim::Prog::MoveTowards::Config(
					Sim::Prog::MoveTowards::DtPosition,
					pos)
			);
		
		return true;
	}
} }
