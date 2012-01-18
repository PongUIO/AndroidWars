#include "BotAbility.h"

#include "../Simulation.h"
#include "../Bot.h"
#include "../Player.h"
#include "../data/BotD.h"

namespace Sim {
	BotAbility::BotAbility()
	{}
	
	BotAbility::~BotAbility()
	{}
	
	void BotAbility::initialize(Bot* host)
	{
		mHost = host;
	}
	
	/**
	 * Executes a function for a single ability list.
	 */
	template<class Func>
	void BotAbility::executeStepPartList(BotAbility::AbilityList& abl, Func F, bool canRemove)
	{
		for(AbilityList::iterator i=abl.begin(); i!=abl.end();) {
			Ability *ability = mHost->mSim->getState().getAbilityFactory()
				.getAbility(*i);
			
			if(ability && ability->isActive()) {
				F(ability);
			}
			
			if(!ability || ability->isFinished()) {
				if(canRemove)
					i = abl.erase(i);
				else
					i++;
			} else
				i++;
		}
	}
	
	/**
	 * Executes an ability function for a step part. This function
	 * calls the \c executeStepPartList function for the ability lists of the
	 * player, then the bot itself.
	 */
	template<class Func>
	void BotAbility::executeStepPart(Func f, bool canRemove)
	{
		executeStepPartList(mHost->getPlayerPtr()->mGlobalAbilities, f, false);
		executeStepPartList(mAbilityList, f, canRemove);
	}
	
	void BotAbility::prepareStep(double delta)
	{
		mAvailProgram.clear();
		
		mAvailProgram.mergeWith(mHost->getPlayerPtr()->mBasePrograms);
		mAvailProgram.mergeWith(mHost->getTypePtr()->baseProgram);
		mAvailProgram.excludeSet(mHost->getTypePtr()->excludeProgram);
		
		executeStepPart(boost::bind(&BotAbility::startAbility, this, _1));
		executeStepPart(boost::bind(&Ability::prepareStep, _1, delta, mHost) );
	}
	
	void BotAbility::startAbility(Ability* ability)
	{
		if(mAbilityHasInit.find(ability->getId()) == mAbilityHasInit.end()) {
			ability->reference();
			ability->start(mHost);
			
			mAbilityHasInit.insert(ability->getId());
		}
	}
	
	void BotAbility::endAbility(Ability* ability)
	{
		if(ability->isFinished()) {
			ability->end(mHost);
			ability->dereference();
		}
	}

	
	void BotAbility::updateCpu(double delta)
	{
		executeStepPart(boost::bind(&Ability::updateCpu, _1, delta, mHost) );
	}
	
	void BotAbility::step(double delta)
	{
		executeStepPart(boost::bind(&Ability::step, _1, delta, mHost) );
		executeStepPart(boost::bind(&BotAbility::endAbility, this, _1), true );
	}
	
	void BotAbility::save(Save::BasePtr& fp) const
	{
		fp.writeCtr(mAbilityList);
		fp.writeUnorderedSet(mAbilityHasInit);
	}
	
	void BotAbility::load(Save::BasePtr& fp)
	{
		fp.readCtr(mAbilityList);
		fp.readUnorderedSet(mAbilityHasInit);
	}
}
