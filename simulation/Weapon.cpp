#include "Simulation.h"
#include "Weapon.h"

#include "data/WeaponD.h"

namespace Sim {
	WeaponFactory::WeaponFactory(Simulation *sim) :
		DefaultUidFactory<Weapon>(sim)
	{}
	
	WeaponFactory::~WeaponFactory()
	{}
	
	const DataBehaviourT<Weapon>::Behaviour* WeaponFactory::getBehaviourFromId(uint32_t id) const
	{	return mSim->getData().getWeaponDb().getType(id); }
	
	const DataBehaviourT<Weapon>::Behaviour* WeaponFactory::getBehaviourFromName(const std::string& name) const
	{	return mSim->getData().getWeaponDb().getType(name); }
	
	
	// WeaponBox
	// 
	// 
	void WeaponBox::save(Save::BasePtr& fp)
	{
		fp.writeInt<uint32_t>(mData.size());
		for(WeaponVec::iterator i=mData.begin(); i!=mData.end(); ++i) {
			fp.writeInt<uint32_t>(*i);
		}
	}
	
	void WeaponBox::load(Save::BasePtr& fp)
	{
		uint32_t count = fp.readInt<uint32_t>();
		for(uint32_t i=0; i<count; i++) {
			uint32_t id = fp.readInt<uint32_t>();
			mData.push_back(id);
		}
	}
}
