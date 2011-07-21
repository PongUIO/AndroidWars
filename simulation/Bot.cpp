#include <stdio.h>

#include "Bot.h"
#include "World.h"
#include "State.h"
#include "Simulation.h"

namespace Sim {
	// Bot
	//
	//
	Bot::Bot(Simulation *sim, uint32_t id, const Config &cfg) :
		mId(id),
		mSide(cfg.side),
		mType(cfg.type),
		mSim(sim)
	{
		mBody.mPos = cfg.pos;
		
		mTypePtr = &mSim->getData().getBotDb().getBot(mType);
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
		mBody.save(fp);
		fp.writeInt(mSide);
	}
	
	// BotFactory
	//
	//
	BotFactory::BotFactory(Simulation *sim)
		: Factory<BotFactory,Bot>(this), mSim(sim)
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
	
	void BotFactory::save(Save::BasePtr &fp)
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
