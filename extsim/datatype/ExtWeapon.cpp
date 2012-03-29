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
	ExtWeapon::ExtWeapon()
	{}

	ExtWeapon::~ExtWeapon()
	{}
	
	void ExtWeapon::loadBlock(Script::Block& block, TypeRule* rule)
	{
		ExtBaseDataObj::loadBlock(block, rule);
		
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
