#include <stdio.h>

#include "Bot.h"
#include "Simulation.h"

namespace Sim {

	Bot::Bot(uint32_t id, uint32_t side) : mId(id), mSide(side) {}
	Bot::~Bot() {}

	void Bot::step(double stepTime)
	{
		mBody.step(stepTime);
	}

	BotFactory::BotFactory(Simulation *sim)
		: Factory<Bot>(sim)
	{
	}

	BotFactory::~BotFactory()
	{
	}

	uint32_t BotFactory::createBot(uint32_t side)
	{
		uint32_t id = newId();
		
		addObj(new Bot(id, side));
		
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

}
