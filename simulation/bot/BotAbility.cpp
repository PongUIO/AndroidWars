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
	void BotAbility::executeStepPartList(BotAbility::AbilityList& abl, Func F)
	{
		for(AbilityList::iterator i=abl.begin(); i!=abl.end();) {
			Ability *ability = mHost->mSim->getState().getAbilityFactory()
				.getAbility(*i);
			
			if(ability && ability->isActive()) {
				F(ability);
			}
			
			if(!ability || ability->isDead())
				i = abl.erase(i);
			else
				i++;
		}
	}
	
	/**
	 * Executes an ability function for a step part. This function
	 * calls the \c executeStepPartList function for the ability lists of the
	 * player, then the bot itself.
	 */
	template<class Func>
	void BotAbility::executeStepPart(Func f)
	{
		executeStepPartList(mHost->getPlayerPtr()->mGlobalAbilities, f);
		executeStepPartList(mAbilityList, f);
	}
	
	void BotAbility::prepareStep(double delta)
	{
		mAvailProgram.clear();
		
		mAvailProgram.mergeWith(mHost->getPlayerPtr()->mBasePrograms);
		mAvailProgram.mergeWith(mHost->getTypePtr()->baseProgram);
		mAvailProgram.excludeSet(mHost->getTypePtr()->excludeProgram);
		
		executeStepPart(boost::bind(&Ability::prepareStep, _1, delta, mHost) );
	}
	
	void BotAbility::updateCpu(double delta)
	{
		executeStepPart(boost::bind(&Ability::updateCpu, _1, delta, mHost) );
	}
	
	void BotAbility::step(double delta)
	{
		executeStepPart(boost::bind(&Ability::step, _1, delta, mHost) );
	}
	
	void BotAbility::save(Save::BasePtr& fp)
	{
		fp.writeInt<uint32_t>(mAbilityList.size());
		for(AbilityList::iterator i=mAbilityList.begin();
			i!=mAbilityList.end(); i++) {
			fp.writeInt<uint32_t>(*i);
		}
	}
	
	void BotAbility::load(Save::BasePtr& fp)
	{
		uint32_t count = fp.readInt<uint32_t>();
		while( (count--) > 0) {
			mAbilityList.push_back(fp.readInt<uint32_t>());
		}
	}
}
