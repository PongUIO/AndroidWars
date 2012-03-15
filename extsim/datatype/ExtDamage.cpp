#include "ExtDamage.h"

#include "../../simulation/Simulation.h"
#include "../ExtSim.h"

namespace ExtS {
	ExtDamageData::ExtDamageData(ExtSim & esim): ExtBaseData(esim)
	{}
	
	ExtDamageData::~ExtDamageData()
	{}
	
	void ExtDamageData::loadBlock(Script::Block& block)
	{
		Sim::DamageDatabase &damageDb = mExtSim.getSim().getData().getDamageDb();
		
		const std::string &name = block.getDataFirst("Name");
		if(name.empty() || damageDb.getDamage(name)!=0)
			return;
		
		damageDb.newDamage(name);
	}
	
	void ExtDamageData::postProcess()
	{}
	
}
