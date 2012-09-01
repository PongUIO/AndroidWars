#include "MoveTowards.h"

#include "../../ExtSim.h"

#include "../../param/Position.h"
#include "../../param/ValRange.h"

namespace exts { namespace prog {
	MoveTowards::MoveTowards(ExtSim& esim): TypeRule(esim)
	{
		allocateParam(PiMax);
		setParam(PiPosition, new PositionParam("Position"));
		setParam(PiDuration, new ValRange<uint32_t>("Duration"));
	}
	
	MoveTowards::~MoveTowards() {}
	
	Sim::IdType MoveTowards::registerSimData(const std::string& name) const
	{
		return mExtSim.getSim().getData().getProgramDb().
		registerImpl<Sim::Prog::MoveTowards>(name);
	}
	
	void MoveTowards::registerInput(ParamList* param) const
	{	allocateId(param, 1); }
	
	void MoveTowards::makeInput(const ParamList* param) const
	{
		_EXTS_PARAM(PositionParam, pos, PiPosition)
		_EXTS_PARAM(ValRange<uint32_t>, duration, PiDuration)
		
		Sim::Prog::MoveTowards::Config cfg = Sim::Prog::MoveTowards::Config(
			Sim::Prog::MoveTowards::DtPosition,pos->getVal()
		);
		
		Sim::Prog::MoveTowards *mt = mExtSim.getSim().getState().getProgramFactory()
		.createImpl<Sim::Prog::MoveTowards>(cfg, mObjectId, param->getAllocId(0));
		
		mt->setRunningTime(duration->getVal());
	}
} }
