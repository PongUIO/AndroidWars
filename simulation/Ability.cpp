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
		UidFactory<Ability>(), mSim(sim)
	{}
	
	AbilityFactory::~AbilityFactory()
	{}
	
	void AbilityFactory::startup()
	{}
	
	void AbilityFactory::shutdown()
	{}
	
	uint32_t AbilityFactory::getAbilityTypeId(const std::string& name)
	{ return mSim->getData().getAbilityDb().getTypeIdOf(name); }
	
	uint32_t AbilityFactory::getAbilityTypeNoId()
	{ return mSim->getData().getAbilityDb().NoId(); }

	
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
	
	void AbilityFactory::saveObj(Ability *obj, Save::BasePtr& fp)
	{
		fp.writeInt<uint32_t>(obj->getTypeId());
		obj->save(fp);
	}
	
	Ability* AbilityFactory::loadObj(uint32_t internalId, Save::BasePtr& fp)
	{
		uint32_t abilType = fp.readInt<uint32_t>();
		
		const AbilityDatabase::Behaviour *abilData =
			mSim->getData().getAbilityDb().getType(abilType);
		
		Ability *ability = abilData->createObj(mSim, internalId);
		ability->load(fp);
		
		return ability;
	}
}
