#include "ExtWeapon.h"

#include "../ExtSim.h"

#include "../../simulation/Simulation.h"
#include "../../simulation/data/WeaponD.h"

namespace ExtS {
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
	ExtWeapon::ExtWeapon(ExtSim *esim) : ExtBaseDataObj(esim)
	{}

	ExtWeapon::~ExtWeapon()
	{}
	
	void ExtWeapon::loadNode(Nepeta::Node& node,
		Sim::IdType simTypeId, TypeRule* rule)
	{
		ExtBaseDataObj::loadNode(node, simTypeId, rule);
		
		node.readChain().
			node("Size").arg(mSizeType).endnode().
			node("BaseDamage").arg(mBaseDamageStr).endnode();
	}
	
	void ExtWeapon::postProcess(ExtSim& extsim)
	{
		ExtBaseDataObj::postProcess(extsim);
		
		mBaseDamageType = extsim.getSim().getData().
			getDamageDb().getIdOf(mBaseDamageStr);
	}
	
}
