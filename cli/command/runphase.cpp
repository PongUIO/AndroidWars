#include "runphase.h"
#include "../SimMgr.h"

runphase::runphase(SimMgr &tmgr): mgr(tmgr)
{

}

runphase::~runphase()
{

}

void runphase::run(const Nepeta::Node& node)
{
	Sim::Simulation &sim = mgr.getExtSim().getSim();
	sim.startPhase();
	while(sim.hasPhaseStep())
		sim.step();
	
	sim.endPhase();
	

}
