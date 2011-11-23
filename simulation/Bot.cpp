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
	void Bot::step(double stepTime)
	{
		handleInput();
		mState.mBody.step(stepTime);
		
		World &world = mSim->getState().getWorld();
		World::ColResult res;
		res = world.collide(mState.mBody.mPos, mState.mBody.mVel, getTypePtr()->getCollision());
		if(res.colRes.isCol) {
			mState.mBody.mPos += res.colRes.getOrp();
		}
	}
	
	const Sim::BotD* Bot::getTypePtr()
	{
		return mSim->getData().getBotDb().getType(mState.mType);
	}
	
	void Bot::save(Save::BasePtr& fp)
	{	mState.save(fp);	}

	void Bot::load(Save::BasePtr& fp)
	{	mState.load(fp);	}
	
	void Bot::State::save(Save::BasePtr &fp)
	{
		fp.writeInt<uint32_t>(mSide);
		fp.writeInt<uint32_t>(mType);
		
		mBody.save(fp);
		mWeaponBox.save(fp);
		
		mCurInput.save(fp);
		mInput.save(fp);
	}
	
	void Bot::State::load(Save::BasePtr& fp)
	{
		mSide = fp.readInt<uint32_t>();
		mType = fp.readInt<uint32_t>();
		
		mBody.load(fp);
		mWeaponBox.load(fp);
		
		mCurInput.load(fp);
		mInput.load(fp);
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
				bot->mState.mInput.addInput(input);
		}
	}
	
	void BotFactory::endPhase()
	{
	}
	
	void BotFactory::save(Save::BasePtr& fp)
	{
		mInput.save(fp);
		Sim::DefaultFactory< Sim::Bot >::save(fp);
	}

	void BotFactory::load(Save::BasePtr& fp)
	{
		mInput.load(fp);
		Sim::DefaultFactory< Sim::Bot >::load(fp);
	}
	
	// BotInput
	//
	//
	void BotInput::save(Save::BasePtr &fp)
	{
		fp.writeInt<uint32_t>(botId);
		fp.writeInt<uint32_t>(stepCount);
		fp.writeInt<uint32_t>(type);
		
		switch(type)
		{
			case Move:
				fp.writeVec(dir);
				break;
				
			case Shoot:
				fp.writeVec(dir);
				fp.writeInt<int32_t>(iparam[0]);
				break;
		}
	}
	
	void BotInput::load(Save::BasePtr& fp)
	{
		botId = fp.readInt<uint32_t>();
		stepCount = fp.readInt<uint32_t>();
		type = (InputType)fp.readInt<uint32_t>();
		
		switch(type)
		{
			case Move:
				dir = fp.readVec();
				break;
				
			case Shoot:
				dir = fp.readVec();
				iparam[0] = fp.readInt<int32_t>();
				break;
		}
	}
}
