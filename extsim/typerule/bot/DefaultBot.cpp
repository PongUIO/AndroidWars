#include "../../ExtSim.h"
#include "DefaultBot.h"

namespace ExtS { namespace Bot {
	Sim::IdType DefaultBot::registerSimData(ExtSim& esim, const std::string& name)
	{
		return esim.getSim().getData().getBotDb().registerImpl<Sim::Bot>(name);
	}
} }
