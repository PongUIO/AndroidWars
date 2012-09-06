#include "Kill.h"

#include "../../ExtSim.h"

#include "../../param/IdParam.h"

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
	
	void Kill::registerInput(ParamList *param) const
	{	allocateId(param, 1); }
	
	void Kill::makeInput(const ParamList* param) const
	{
		_EXTS_PARAM(IdList<Sim::Program>, prog, 0)
		
		Sim::Prog::Kill::Config cfg = Sim::Prog::Kill::Config(prog->getId());
		mExtSim.getSim().getState().getProgramFactory().
		createImpl<Sim::Prog::Kill>(cfg, mObjectId, param->getAllocId(0));
	}
} }
