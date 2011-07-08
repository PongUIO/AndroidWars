#include <stdio.h>

#include "Bot.h"
#include "World.h"
#include "State.h"
#include "Simulation.h"

namespace Sim {
	// Bot
	//
	//
	Bot::Bot(Simulation *sim, Collision *col, uint32_t id, const Config &cfg) :
		mId(id),
		mSide(cfg.side),
		mCol(col),
		mSim(sim)
	{
		mBody.mPos = cfg.pos;
	}
	
	Bot::~Bot() {}

	void Bot::step(double stepTime)
	{
		handleInput();
		mBody.step(stepTime);
		
		World &world = mSim->getState().getWorld();
		World::ColResult res;
		res = world.collide(mBody.mPos, mBody.mVel, mCol);
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
		
		Collision::ColPoints pts;
		pts.push_back( Vector(0,0) );
		pts.push_back( Vector(0,1) );
		pts.push_back( Vector(1,1) );
		pts.push_back( Vector(1,0) );
		
		addObj(new Bot(mSim, new Collision(pts), id, cfg));
		
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
