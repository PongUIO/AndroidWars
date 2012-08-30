#include "startup.h"
#include "../SimMgr.h"

startup::startup(SimMgr &tmgr): mgr(tmgr)
{
//intentionally left black
}

startup::~startup()
{

}

void startup::run(const Nepeta::Node& node)
{
	mgr.getExtSim().startup();

}
