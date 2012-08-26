#include "Kill.h"

#include "../../ExtSim.h"

#include "../../param/IdList.h"

namespace exts { namespace prog {
	Kill::Kill(ExtSim& esim): TypeRule(esim)
	{
		registerParam(new IdList<Sim::Program>("Program"));
	}
	
	Kill::~Kill() {}
	
	Sim::IdType Kill::registerSimData(const std::string& name) const
	{
		return mExtSim.getSim().getData().getProgramDb().
		registerImpl<Sim::Prog::Kill>(name);
	}
	
	void Kill::makeInput(const ParamList* param) const
	{
		const IdList<Sim::Program> *prog = param->getParamT<IdList<Sim::Program> >(0);
		
		Sim::Prog::Kill::Config cfg = Sim::Prog::Kill::Config(prog->getId());
		mExtSim.getSim().getInput().getProgramInput()
		.buildInputImpl<Sim::Prog::Kill>(cfg, mObjectId);
	}
} }
