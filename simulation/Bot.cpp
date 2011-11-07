#include <stdio.h>

#include "Bot.h"
#include "Weapon.h"
#include "World.h"
#include "State.h"
#include "Simulation.h"

namespace Sim {
	// Bot
	//
	//
	Bot::Bot(Simulation *sim, uint32_t id, const Config &cfg) :
		mId(id), mSim(sim)
	{
		loadConfig(cfg);
	}
	
	Bot::~Bot() {}

	void Bot::step(double stepTime)
	{
		handleInput();
		mBody.step(stepTime);
		
		World &world = mSim->getState().getWorld();
		World::ColResult res;
		res = world.collide(mBody.mPos, mBody.mVel, mTypePtr->getCollision());
		if(res.colRes.isCol) {
			mBody.mPos += res.colRes.getOrp();
		}
	}
	
	void Bot::save(Save::BasePtr &fp)
	{
		fp.writeInt<uint32_t>(mSide);
		fp.writeInt<uint32_t>(mType);
		
		mBody.save(fp);
	}
	
	void Bot::load(Save::BasePtr& fp)
	{
		Config cfg;
		
		cfg.side = fp.readInt<uint32_t>();
		cfg.type = fp.readInt<uint32_t>();
		
		cfg.body.load(fp);
		
		loadConfig(cfg);
	}
	
	void Bot::loadConfig(const Sim::Bot::Config& cfg)
	{
		mBody = cfg.body;
		
		mType = cfg.type;
		mTypePtr = mSim->getData().getBotDb().getType(mType);
	}
	
	// BotFactory
	//
	//
	BotFactory::BotFactory(Simulation *sim)
		: DefaultFactory<Bot>(sim)
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
		
		addObj(new Bot(mSim, id, cfg));
		
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
