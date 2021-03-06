#include "ExtDamage.h"

#include "../../simulation/Simulation.h"
#include "../ExtSim.h"

namespace exts {
	ExtDamageData::ExtDamageData(ExtSim & esim) :
		ExtDataComponent(), mExtSim(esim)
	{}
	
	ExtDamageData::~ExtDamageData()
	{}
	
	void ExtDamageData::loadNode(const Nepeta::Node& node)
	{
		Sim::DamageDatabase &damageDb = mExtSim.getSim().getData().getDamageDb();
		
		const std::string &name = node.getNodeFirst("Name");
		if(name.empty() || damageDb.getDamage(name)!=0)
			return;
		
		damageDb.newDamage(name);
	}
	
	void ExtDamageData::postProcess()
	{}
	
}
