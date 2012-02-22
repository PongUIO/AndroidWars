#include "Ability.h"
#include "data/AbilityD.h"
#include "Simulation.h"

#include "ability/AbilityInclude.h"

namespace Sim {
	// Ability
	//
	//
#define _SIM_ABILITY_DEF(baseClass,	type, name) \
	const std::string &Abil::baseClass::getTypeName() {\
		static std::string typeName = name; \
		return typeName; \
	}
#include "ability/AbilityDef.def"
#undef _SIM_ABILITY_DEF
	
	void Ability::abort(bool force) { mIsFinished=true; }
	void Ability::toggleSuspend(bool isSuspended, bool force)
	{ mIsActive = !isSuspended; }
	
	void Ability::injectDelay(int32_t delay, bool force)
	{
		int32_t newDelay = mDelay+delay;
		if(newDelay < 0)
			newDelay = 0;
		
		mDelay = newDelay;
	}
	
	void Ability::setDuration(uint32_t duration, bool force)
	{
		if(duration < 0)
			duration = -1;
		
		mDuration = duration;
	}

	
	void Ability::updateInternal()
	{
		if(mDuration > 0 && mDuration!=uint32_t(-1))
			mDuration--;
		if(mDelay > 0)
			mDelay--;
	}
	
	// AbilityFactory
	//
	//
	AbilityFactory::AbilityFactory(Simulation *sim) :
		DefaultUidFactory<Ability>(sim)
	{}
	
	AbilityFactory::~AbilityFactory()
	{}
	
	void AbilityFactory::startup()
	{}
	
	void AbilityFactory::shutdown()
	{
		killAll();
	}
	
	void AbilityFactory::startPhase()
	{}
	
	void AbilityFactory::endPhase()
	{}
	
	void AbilityFactory::step(double stepTime)
	{
		factoryCall(boost::bind(&Ability::updateInternal, _1));
		
		// Force cleanup of dead abilities
		cleanDead();
	}
	
	void AbilityFactory::save(Save::BasePtr& fp)
	{	UidFactory<Ability>::save(fp); }

	void AbilityFactory::load(Save::BasePtr& fp)
	{	UidFactory<Ability>::load(fp); }
	
	void AbilityFactory::saveObj(Ability* obj, Save::BasePtr& fp)
	{
		DefaultUidFactory<Ability>::saveObj(obj, fp);
		obj->saveSys(fp);
	}

	Ability* AbilityFactory::loadObj(IdType internalId, Save::BasePtr& fp)
	{
		Ability *obj = DefaultUidFactory<Ability>::loadObj(internalId,fp);
		obj->loadSys(fp);
		return obj;
	}

}
