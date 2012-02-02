#include "IdList.h"

#include "../../simulation/data/ArmorD.h"
#include "../ExtSim.h"

namespace ExtS {
	template<>
	Sim::IdType translateNameToId<Sim::ArmorD>(ExtSim& extsim, const std::string& str)
	{	return extsim.getSim().getData().getArmorDb().getIdOf(str); }
}
