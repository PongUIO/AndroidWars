#include "Weapon.h"

#include "../../simulation/Simulation.h"
#include "../../simulation/data/WeaponD.h"

namespace ExtS {
	WeaponData::WeaponData(Sim::Simulation& sim): BaseData(sim)
	{}
	
	WeaponData::~WeaponData()
	{}
	
	void WeaponData::loadBlock(Script::Block& block)
	{
		Sim::WeaponDatabase &weapDb = mSim.getData().getWeaponDb();
		
		const std::string &name = block.getDataFirst("Name");
		if(name.empty() || weapDb.getType(name)!=0)
			return;
		
	}
	
}
