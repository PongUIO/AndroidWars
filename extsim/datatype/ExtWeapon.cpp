#include "ExtWeapon.h"

#include "../ExtSim.h"

#include "../../simulation/Simulation.h"
#include "../../simulation/data/WeaponD.h"

namespace exts {
	// ExtWeaponData
	//
	//
	ExtWeaponData::ExtWeaponData(ExtSim &esim) :
		DefaultExtData<ExtWeapon>(esim)
	{
		
	}
	
	ExtWeaponData::~ExtWeaponData()
	{
		
	}
	
	// ExtProgram
	//
	//
	ExtWeapon::ExtWeapon(ExtSim &esim, Sim::IdType id) : ExtDataObjBase(esim,id)
	{}

	ExtWeapon::~ExtWeapon()
	{}
	
	void ExtWeapon::loadNode(const Nepeta::Node& node)
	{
		ExtDataObjBase::loadNode(node);
		
		node.readChain().
			node("Size").arg(mSizeType).endnode().
			node("BaseDamage").arg(mBaseDamageStr).endnode();
	}
	
	void ExtWeapon::postProcess()
	{
		ExtDataObjBase::postProcess();
		
		mBaseDamageType = mExtSim.getSim().getData().
			getDamageDb().getIdOf(mBaseDamageStr);
	}
	
}
