#include "include.h"


include::include(CmdManager &tcmd) : mcmd(tcmd)
{
	
}

include::~include()
{
	
}

void include::run(const Nepeta::Node& node)
{
	mcmd.execfile(node.getArg());
}
