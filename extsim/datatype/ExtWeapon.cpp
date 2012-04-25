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
	
	void ExtWeapon::loadBlock(Script::Block& block,
		Sim::IdType simTypeId, TypeRule* rule)
	{
		ExtBaseDataObj::loadBlock(block, simTypeId, rule);
		
		mSizeType = block.getDataFirst("Size");
		mBaseDamageStr = block.getDataFirst("BaseDamage");
	}
	
	void ExtWeapon::postProcess(ExtSim& extsim)
	{
		ExtBaseDataObj::postProcess(extsim);
		
		mBaseDamageType = extsim.getSim().getData().
			getDamageDb().getIdOf(mBaseDamageStr);
	}
	
}
