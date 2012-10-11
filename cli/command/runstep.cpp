#include "runstep.h"
#include "../SimMgr.h"

runstep::runstep(SimMgr &tmgr): mgr(tmgr)
{

}

runstep::~runstep()
{

}

void runstep::run(const Nepeta::Node& node)
{
	Sim::Simulation &sim = mgr.getExtSim().getSim();
	
	Sim::State::StateType state = sim.getState().getStateType();
	
	if(state == Sim::State::StIdle)
		sim.startPhase();
	if(state == Sim::State::StInPhase)
		sim.step();
	if(state == Sim::State::StEndPhase)
		sim.endPhase(true);
	
}
