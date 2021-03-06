#include "Simulation.h"
#include "Weapon.h"

#include "data/WeaponD.h"

namespace Sim {
	WeaponFactory::WeaponFactory(Simulation *sim) :
		DefaultUidFactory<Weapon>(sim)
	{}
	
	WeaponFactory::~WeaponFactory()
	{}
	
	void WeaponFactory::saveObj(Weapon* obj, Save::BasePtr& fp)
	{
		DefaultUidFactory<Weapon>::saveObj(obj, fp);
		obj->saveSys(fp);
	}

	Weapon* WeaponFactory::loadObj(IdType internalId, Save::BasePtr& fp)
	{
		Weapon *obj = DefaultUidFactory<Weapon>::loadObj(internalId, fp);
		obj->loadSys(fp);
		return obj;
	}

	
	
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
