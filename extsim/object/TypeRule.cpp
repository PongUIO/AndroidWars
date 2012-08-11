#include "TypeRule.h"
#include "ParamList.h"

#include "../CommonLoad.h"

namespace exts {
	void TypeRule::load(const Nepeta::Node& node)
	{
		const Nepeta::Node &param = node.getNode(Load::Param);
		const Nepeta::Node &constraint = node.getNode(Load::Constraint);
		
		for(size_t i=0; i<mParam.size(); ++i) {
			mParam.getParam(i)->readNode(param, constraint);
		}
	}
	
	void TypeRule::registerParam(RuleParameter* param)
	{ mParam.addParam(param); }
	
	void TypeRule::setId(Sim::IdType id)
	{ mId = id; mParam.setRefTypeRuleId(id); }
	
	ParamList* TypeRule::makeParam() const
	{ return new ParamList(mParam); }
	
	bool TypeRule::checkConstrained(const ParamList* param) const
	{
		if(mParam.size() != param->size())
			return false;
		
		// Compare each parameter to test against the reference parameters
		// if any parameter is not constrained, return false 
		for(size_t i=0; i<mParam.size(); ++i) {
			if(!mParam.getParam(i)->isConstrained(
			param->getParam(i), mExtSim)) {
				return false;
			}
		}
		
		return true;
	}

}
