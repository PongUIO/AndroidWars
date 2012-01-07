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
	
	const DataBehaviourT< Ability >::Behaviour* AbilityFactory::getBehaviourFromId(uint32_t id) const
	{	return mSim->getData().getAbilityDb().getType(id); }
	
	const DataBehaviourT< Ability >::Behaviour* AbilityFactory::getBehaviourFromName(const std::string& name) const
	{	return mSim->getData().getAbilityDb().getType(name); }
	
	void AbilityFactory::startPhase()
	{}
	
	void AbilityFactory::endPhase()
	{}
	
	void AbilityFactory::step(double stepTime)
	{}
	
	void AbilityFactory::save(Save::BasePtr& fp)
	{	UidFactory<Ability>::save(fp); }

	void AbilityFactory::load(Save::BasePtr& fp)
	{	UidFactory<Ability>::load(fp); }
}
