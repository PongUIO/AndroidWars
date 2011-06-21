#include <stdio.h>

#include "Bot.h"
#include "Simulation.h"
#include "Sync.h"

namespace Sim {
	// Bot
	//
	//
	Bot::Bot(uint32_t id, const Config &cfg) :
		mId(id),
		mSide(cfg.side)
	{
		mBody.mPos = cfg.pos;
	}
	
	Bot::~Bot() {}

	void Bot::step(double stepTime)
	{
		if(isIdle() && mInput.hasInput())
			mCurInput = mInput.nextInput();
		
		if(!isIdle()) {
			mCurInput.stepCount--;
		}
		
		mBody.step(stepTime);
	}
	
	void Bot::checksum(Sync& sync)
	{
		mBody.checksum(sync);
	}
	
	// BotFactory
	//
	//
	BotFactory::BotFactory(Simulation *sim)
		: Factory<Bot>(sim)
	{}

	BotFactory::~BotFactory()
	{}
	
	void BotFactory::startup()
	{
		
	}
	
	void BotFactory::shutdown()
	{
		killAll();
	}
	
	uint32_t BotFactory::createBot(const Bot::Config &cfg)
	{
		uint32_t id = newId();
		
		addObj(new Bot(id, cfg));
		
		return id;
	}
	
	void BotFactory::startPhase()
	{
		// Feed our bots with input
		while(mInput.hasInput()) {
			BotInput input = mInput.nextInput();
			
			Bot *bot = getObject(input.botId);
			if(bot)
				bot->mInput.addInput(input);
		}
	}
	
	void BotFactory::endPhase()
	{
	}
	
	void BotFactory::checksum(Sync& sync)
	{
		for(ObjVec::iterator i=mData.begin(); i!=mData.end(); i++) {
			(*i)->checksum(sync);
		}
	}
	
	void BotFactory::copyState(State& state)
	{
		copyFactory(state.getBotFactory());
	}
}
