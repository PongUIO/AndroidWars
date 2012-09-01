#include "BaseBot.h"

#include "../../ExtSim.h"

#include "../../param/Position.h"

namespace exts { namespace bot {
	BaseBot::BaseBot(ExtSim& esim) : TypeRule(esim)
	{
		allocateParam(PiMax);
		setParam(PiPosition, new PositionParam("Position"));
	}
	
	BaseBot::~BaseBot()
	{}
	
	Sim::IdType BaseBot::registerSimData(const std::string& name) const
	{
		return mExtSim.getSim().getData().getBotDb().
		registerImpl<Sim::Bot>(name);
	}

	void BaseBot::registerInput(ParamList* param) const
	{	allocateId(param, 1); }

	void BaseBot::makeInput(const ParamList* param) const
	{
		_EXTS_PARAM(PositionParam, pos, PiPosition)
		
		Sim::Bot::Config cfg;
		cfg.mSide = 0;
		cfg.mBody.mPos = pos->getVal();
		
		mExtSim.getSim().getState().getBotFactory().createImpl<Sim::Bot>(
			cfg,
			mObjectId,
			param->getAllocId(0)
		);
	}
} }
