#include "Damage.h"

#include "../../simulation/Simulation.h"
#include "../ExtSim.h"

namespace ExtS {
	DamageData::DamageData(ExtSim & esim): BaseData(esim)
	{}
	
	DamageData::~DamageData()
	{}
	
	void DamageData::loadBlock(Script::Block& block)
	{
		Sim::DamageDatabase &damageDb = mExtSim.getSim().getData().getDamageDb();
		
		const std::string &name = block.getDataFirst("Name");
		if(name.empty() || damageDb.getDamage(name)!=0)
			return;
		
		damageDb.newDamage(name);
	}
	
	void DamageData::postProcess()
	{}
	
}
