#include "Simulation.h"
#include "Weapon.h"

#include "data/WeaponD.h"

namespace Sim {
	WeaponFactory::WeaponFactory(Simulation *sim) :
		DefaultUidFactory<Weapon>(sim)
	{}
	
	WeaponFactory::~WeaponFactory()
	{}
	
	const DataBehaviourT<Weapon>::Behaviour* WeaponFactory::getBehaviourFromId(IdType id) const
	{	return mSim->getData().getWeaponDb().getType(id); }
	
	const DataBehaviourT<Weapon>::Behaviour* WeaponFactory::getBehaviourFromName(const std::string& name) const
	{	return mSim->getData().getWeaponDb().getType(name); }
	
	
	// WeaponBox
	// 
	//
	void WeaponBox::save(Save::BasePtr& fp) const
	{
		fp.writeCtr(mData);
	}
	
	void WeaponBox::load(Save::BasePtr& fp)
	{
		fp.readCtr(mData);
	}
}
