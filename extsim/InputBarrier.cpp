#include "InputBarrier.h"

#include "ExtSim.h"
#include "object/TypeRule.h"
#include "object/ParamList.h"

namespace exts {
	InputBarrier::InputBarrier(ExtSim& esim) : mExtSim(esim)
	{}

	InputBarrier::~InputBarrier()
	{	discardInput(); }
	
	void InputBarrier::startup()
	{}

	void InputBarrier::shutdown()
	{	discardInput(); }

	bool InputBarrier::registerInput(ParamList* param)
	{
		if(!param)
			return false;
		
		bool isConstrained = param->isConstrained(mExtSim);
		if(isConstrained)
			mInput.push_back(param);
		
		return isConstrained;
	}
	
	/**
	 * @brief Sends the current input buffer to the simulation
	 */
	void InputBarrier::dispatchInput()
	{
		for(ParamListVec::iterator i=mInput.begin(); i!=mInput.end(); ++i) {
			const TypeRule *rule = mExtSim.getTypeRuleMgr()
			.getRule((*i)->getTypeRuleId());
			
			if(rule)
				rule->makeInput(*i);
			
			delete *i;
		}
		mInput.clear();
	}
	
	/**
	 * @brief Discards the active input buffer
	 */
	void InputBarrier::discardInput()
	{
		for(ParamListVec::iterator i=mInput.begin(); i!=mInput.end(); ++i)
			delete *i;
	}

}
