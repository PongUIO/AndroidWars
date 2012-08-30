#include "shutdown.h"
#include "../SimMgr.h"

shutdown::shutdown(SimMgr &tmgr): mgr(tmgr)
{

}

shutdown::~shutdown()
{

}

void shutdown::run(const Nepeta::Node& node)
{
	mgr.getExtSim().shutdown();

}
