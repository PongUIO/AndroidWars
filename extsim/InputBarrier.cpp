#include <stdint.h>
#include <stdio.h>

#include "InputBarrier.h"

#include "ExtSim.h"
#include "object/TypeRule.h"
#include "object/ParamList.h"

namespace exts {
	// InputList
	// 
	// 
	InputList::InputList(ExtSim &esim) : mExtSim(esim)
	{}
	
	InputList::~InputList()
	{
		discardInput();
	}
	
	/**
	 * @brief Registers a \c ParamList to the \c InputList
	 * 
	 * @note The \c InputList takes ownership of \p param.
	 */
	bool InputList::registerInput(ParamList* param)
	{
		if(!param)
			return false;
		
		bool isConstrained = param->isConstrained(mExtSim);
		if(isConstrained) {
			const TypeRule *rule = mExtSim.getTypeRuleMgr()
			.getRule(param->getTypeRuleId());
			
			rule->registerInput(param);
			mInput.push_back(param);
		}
		
		return isConstrained;
	}
	
	/**
	 * @brief Discards and deletes all input stored in this \c InputList.
	 */
	void InputList::discardInput()
	{
		for(size_t i=0; i<mInput.size(); ++i)
			delete mInput[i];
		
		mInput.clear();
	}
	
	/**
	 * @brief Inherits all input from the source \c InputList.
	 */
	void InputList::inheritList(InputList& source)
	{
		mInput.insert(mInput.end(),
		source.mInput.begin(), source.mInput.end());
		source.mInput = ParamListVec();
	}
	
	/**
	 * @brief Copies the input from the source \c InputList
	 */
	void InputList::copyList(const InputList& source)
	{
		for(ParamListVec::const_iterator i = source.mInput.begin();
		i!=source.mInput.end(); ++i) {
			registerInput(new ParamList(**i));
		}
	}
	
	void InputList::save(Sim::Save::BasePtr& fp) const
	{
		fp << uint32_t(mInput. size());
		for(ParamListVec::const_iterator i=mInput.begin(); i!=mInput.end(); ++i)
			mExtSim.getTypeRuleMgr().saveParamList(fp, *i);
	}

	void InputList::load(Sim::Save::BasePtr& fp)
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

	
	// InputBarrier
	// 
	// 
	InputBarrier::InputBarrier(ExtSim& esim) : mExtSim(esim), mInput(esim)
	{}

	InputBarrier::~InputBarrier()
	{	discardInput(); }
	
	void InputBarrier::startup()
	{}

	void InputBarrier::shutdown()
	{	discardInput(); }
	
	/**
	 * @brief Copies any input from the \c InputList, preserving its
	 * original contents.
	 */
	void InputBarrier::copyInput(const InputList& il)
	{	mInput.copyList(il); }
	
	/**
	 * @brief Makes the \c InputBarrier take ownership of all the input
	 * in the \c InputList provided.
	 */
	void InputBarrier::applyInput(InputList& il)
	{	mInput.inheritList(il); }
	
	/**
	 * @see InputList::registerInput
	 */
	bool InputBarrier::registerInput(ParamList* param)
	{	return mInput.registerInput(param); }
	
	/**
	 * @brief Dispatches input to the simulation and optionally saves a replay.
	 * 
	 * This is a meta-function for the \c feedInput(), \c commitReplay(),
	 * \c postProcessInput(), and \c discardInput() to simplify individual
	 * calls.
	 */
	void InputBarrier::dispatchInput(bool saveReplay)
	{
		// Feed input
		feedInput();
		
		// Save the replay
		if(saveReplay)
			commitReplay();
		
		// Perform post-processing (updating input agents)
		postProcessInput();
		
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
		for(InputList::ParamListVec::const_iterator i=
		mInput.getParamList().begin();
		i!=mInput.getParamList().end(); ++i) {
			const TypeRule *rule = mExtSim.getTypeRuleMgr()
			.getRule((*i)->getTypeRuleId());
			
			if(rule)
				rule->makeInput(*i);
		}
	}
	
	/**
	 * @brief Performs tasks for after input
	 * 
	 * This is needed because the replay must be committed before the input
	 * agents are updated.
	 */
	void InputBarrier::postProcessInput()
	{
		// Update agent ID allocation
		mExtSim.getAgent().updateAllocAll();
	}
	
	/**
	 * @brief Commits the current stored input to the \c ReplayManager
	 * 
	 * This assumes that the input buffer comes from this phase
	 * (essentially, this is called "right after" \c feedInput()), and
	 * that the simulation has loaded the input, or has processed through
	 * the input.
	 * 
	 * @warning Always call this between a call to \c feedInput() and
	 * \c postProcessInput(), otherwise weird things might happen.
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
		mInput.discardInput();
		
		mExtSim.getAgent().discardAllocAll();
	}
	
	void InputBarrier::saveInput(Sim::Save::BasePtr& fp) const
	{
		fp << mInput;
	}
	
	void InputBarrier::loadInput(Sim::Save::BasePtr& fp)
	{
		fp >> mInput;
	}
	
	void InputBarrier::loadInput(const Sim::Save& save)
	{
		Sim::Save::FilePtr fp(save);
		loadInput(fp);
	}
}
