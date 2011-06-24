#include <stdio.h>

#include "Bot.h"
#include "Simulation.h"

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
		handleInput();
		mBody.step(stepTime);
	}
	
	void Bot::save(Save::Ptr &fp)
	{
		mBody.save(fp);
		fp.writeInt(mSide);
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
	
	void BotFactory::save(Save::Ptr &fp)
	{
		fp.writeInt(mData.size());
		for(ObjVec::iterator i=mData.begin(); i!=mData.end(); i++) {
			(*i)->save(fp);
		}
		fp.writeInt(NoId());
	}
	
	void BotFactory::copyState(State& state)
	{
		BotFactory &other = state.getBotFactory();
		copyFactory(other);
		mInput = other.getInput();
	}
}
