#include <stdint.h>
#include <stdio.h>

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
	void InputBarrier::dispatchInput(bool saveReplay)
	{
		// Feed input
		for(ParamListVec::iterator i=mInput.begin(); i!=mInput.end(); ++i) {
			const TypeRule *rule = mExtSim.getTypeRuleMgr()
			.getRule((*i)->getTypeRuleId());
			
			if(rule)
				rule->makeInput(*i);
		}
		
		// Fast-forward the simulation
		if(saveReplay) {
			mExtSim.getSim().startPhase();
			while(mExtSim.getSim().hasPhaseStep())
				mExtSim.getSim().step();
			mExtSim.getSim().endPhase();
			
			// Commit the replay
			mExtSim.getReplay().commitNewBranch();
		}
		
		// Throw away the spent input
		discardInput();
	}
	
	/**
	 * @brief Discards the active input buffer
	 */
	void InputBarrier::discardInput()
	{
		for(ParamListVec::iterator i=mInput.begin(); i!=mInput.end(); ++i)
			delete *i;
		mInput.clear();
	}
	
	void InputBarrier::save(Sim::Save::BasePtr& fp)
	{
		fp << uint32_t(mInput.size());
		for(ParamListVec::iterator i=mInput.begin(); i!=mInput.end(); ++i)
			mExtSim.getTypeRuleMgr().saveParamList(fp, *i);
	}
	
	void InputBarrier::load(Sim::Save::BasePtr& fp)
	{
		uint32_t count;
		fp >> count;
		
		for(uint32_t i=0; i<count; ++i) {
			ParamList *param = mExtSim.getTypeRuleMgr().loadParamList(fp);
			
			if(param)
				registerInput(param);
			else
				break;
		}
	}
	
	void InputBarrier::load(Sim::Save& save)
	{
		Sim::Save::FilePtr fp(save);
		load(fp);
	}
}
