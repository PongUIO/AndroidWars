#include "Damage.h"

#include "../../simulation/Simulation.h"

namespace ExtS {
	DamageData::DamageData(Sim::Simulation& sim): BaseData(sim)
	{}
	
	DamageData::~DamageData()
	{}
	
	void DamageData::loadBlock(Script::Block& block)
	{
		Sim::DamageDatabase &damageDb = mSim.getData().getDamageDb();
		
		const std::string &name = block.getDataFirst("Name");
		if(name.empty() || damageDb.getDamage(name)!=0)
			return;
		
		damageDb.newDamage(name);
	}
	
	void DamageData::postProcess()
	{}
	
}
