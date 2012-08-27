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
	
	/**
	 * @brief Registers a \c ParamList to the \c InputBarrier
	 * 
	 * @note The \c InputBarrier takes ownership of \p param.
	 */
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
	 * @brief Dispatches input to the simulation and optionally saves a replay.
	 * 
	 * This is a meta-function for the \c feedInput(), \c commitReplay(), and
	 * \c discardInput() to simplify individual calls.
	 */
	void InputBarrier::dispatchInput(bool saveReplay)
	{
		// Feed input
		feedInput();
		
		if(saveReplay)
			commitReplay();
		
		// Throw away the spent input
		discardInput();
	}
	
	/**
	 * @brief Feeds the stored input to the simulation
	 * 
	 * Sends the entire current input buffer to the simulation, the internal
	 * buffer is stored until \c discardInput is called.
	 */
	void InputBarrier::feedInput()
	{
		for(ParamListVec::iterator i=mInput.begin(); i!=mInput.end(); ++i) {
			const TypeRule *rule = mExtSim.getTypeRuleMgr()
			.getRule((*i)->getTypeRuleId());
			
			if(rule)
				rule->makeInput(*i);
		}
	}
	
	/**
	 * @brief Commits the current stored input to the \c ReplayManager
	 * 
	 * This assumes that the input buffer comes from this phase
	 * (essentially, this is called "right after" \c feedInput()), and
	 * that the simulation has loaded the input, or has processed through
	 * the input.
	 */
	void InputBarrier::commitReplay()
	{
		// Fast-forward the simulation if not already performed
		if(mExtSim.getReplay().getActiveNode()->getDepth()
		== mExtSim.getSim().getCurPhase()) {
			mExtSim.getSim().startPhase();
			while(mExtSim.getSim().hasPhaseStep())
				mExtSim.getSim().step();
			mExtSim.getSim().endPhase();
		}
		
		// Commit the replay
		mExtSim.getReplay().commitNewBranch();
	}
	
	/**
	 * @brief Discards the active input buffer
	 * 
	 * Every object in the buffer is deleted.
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
