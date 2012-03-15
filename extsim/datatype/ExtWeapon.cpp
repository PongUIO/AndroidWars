#include "ExtWeapon.h"

#include "../../simulation/Simulation.h"
#include "../../simulation/data/WeaponD.h"

namespace ExtS {
	ExtWeaponData::ExtWeaponData(Sim::Simulation& sim): BaseData(sim)
	{}
	
	ExtWeaponData::~ExtWeaponData()
	{}
	
	void ExtWeaponData::loadBlock(Script::Block& block)
	{
		Sim::WeaponDatabase &weapDb = mSim.getData().getWeaponDb();
		
		const std::string &name = block.getDataFirst("Name");
		if(name.empty() || weapDb.getType(name)!=0)
			return;
		
	}
	
}
