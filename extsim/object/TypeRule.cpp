#include "TypeRule.h"
#include "ParamList.h"

#include "../CommonLoad.h"
#include "../ExtSim.h"

namespace exts {
	void TypeRule::load(const Nepeta::Node& node)
	{
		const Nepeta::Node &param = node.getNode(Load::Param);
		const Nepeta::Node &constraint = node.getNode(Load::Constraint);
		
		for(size_t i=0; i<mParam.size(); ++i) {
			mParam.getParam(i)->readNode(param, constraint);
		}
	}
	
	/**
	 * @brief Allocates a set number of new \c RuleParameter objects for the
	 * reference parameters.
	 */
	void TypeRule::allocateParam(size_t count)
	{
		mParam.getParamVec().resize(mParam.getParamVec().size()+count);
	}
	
	void TypeRule::setParam(size_t index, RuleParameter* param)
	{
		if(index < mParam.size())
			mParam.getParamVec()[index] = param;
	}

	
	void TypeRule::registerParam(RuleParameter* param)
	{ mParam.addParam(param); }
	
	void TypeRule::allocateId(ParamList* param, size_t count) const
	{
		Agent *agent = mExtSim.getAgent().getAgent(param->getAgent());
		if(!agent)
			return; /// @todo Throw exception for illegal agent
		
		while( (count--) > 0) {
			param->allocateId(agent->allocateId());
		}
	}
	
	void TypeRule::setId(Sim::IdType id)
	{ mId = id; mParam.setRefTypeRuleId(id); }
	
	ParamList* TypeRule::makeParam(Sim::IdType agentId) const
	{
		ParamList *tmp = new ParamList(mParam);
		tmp->setAgent(agentId);
		return tmp;
	}
	
	bool TypeRule::checkConstrained(const ParamList* param) const
	{
		if(mParam.size() != param->size())
			return false;
		
		// Compare each parameter to test against the reference parameters
		// if any parameter is not constrained, return false 
		for(size_t i=0; i<mParam.size(); ++i) {
			if(!mParam.getParam(i)->isConstraintDefined())
				continue;
			
			if(!mParam.getParam(i)->isConstrained(
			param->getParam(i), mExtSim)) {
				return false;
			}
		}
		
		return true;
	}

}
