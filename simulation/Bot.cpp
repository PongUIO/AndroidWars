#include <stdio.h>

#include "Bot.h"
#include "Simulation.h"

namespace Sim {

	Bot::Bot(uint32_t id) : mId(id) {}
	Bot::~Bot() {}

	void Bot::step(double stepTime)
	{
		printf("Hello! I am ID %d\n", getId());
		while(mInput.hasInput()) {
			printf("\tI have some input, message: %u\n",
				mInput.nextInput().dbgMessage);
		}
	}

	BotFactory::BotFactory(Simulation *sim)
		: Factory<Bot>(sim)
	{
	}

	BotFactory::~BotFactory()
	{
	}

	uint32_t BotFactory::createBot()
	{
		uint32_t id = newId();
		
		addObj(new Bot(id));
		
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
