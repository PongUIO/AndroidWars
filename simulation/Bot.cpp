#include "Bot.h"
#include "Weapon.h"
#include "World.h"
#include "State.h"
#include "Simulation.h"

namespace Sim {
	// Bot
	//
	//
	void Bot::prepareStep(double stepTime)
	{
		// Update the sensor with data
		SensorState &sensor = mState.mSensor;
		sensor.mTargetBot = FactoryNoId;
		sensor.mWasHit = false;
		
		// Reset the engine
		Engine &engine = mState.mEngine;
		engine.mDirection = 0;
		engine.mStrength = 0;
	}
	
	void Bot::updateCpu(double stepTime)
	{
		mState.mCpu.step(stepTime);
	}
	
	void Bot::step(double stepTime)
	{
		mState.mBody.addMomentum(
			mState.mEngine.mDirection*(mState.mEngine.mStrength*stepTime) );
		
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
		
		mSensor.save(fp);
		
		mBody.save(fp);
		mWeaponBox.save(fp);
		
		mCpu.save(fp);
	}
	
	void Bot::State::load(Save::BasePtr& fp)
	{
		mSide = fp.readInt<uint32_t>();
		mType = fp.readInt<uint32_t>();
		
		mSensor.load(fp);
		
		mBody.load(fp);
		mWeaponBox.load(fp);
		
		mCpu.load(fp);
	}
	
	void Bot::SensorState::save(Save::BasePtr& fp)
	{
		fp.writeInt<uint32_t>(mTargetBot);
		fp.writeInt<uint8_t>(mWasHit);
	}

	void Bot::SensorState::load(Save::BasePtr& fp)
	{
		mTargetBot = fp.readInt<uint32_t>();
		mWasHit = fp.readInt<uint8_t>();
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
	
	void BotFactory::step(double stepTime)
	{
		factoryCall(boost::bind(&Bot::prepareStep, _1, stepTime));
		factoryCall(boost::bind(&Bot::updateCpu, _1, stepTime));
		factoryCall(boost::bind(&Bot::step, _1, stepTime));
	}
	
	
	void BotFactory::startPhase()
	{
	}
	
	void BotFactory::endPhase()
	{
	}
	
	void BotFactory::save(Save::BasePtr& fp)
	{
		Sim::DefaultFactory< Sim::Bot >::save(fp);
	}

	void BotFactory::load(Save::BasePtr& fp)
	{
		Sim::DefaultFactory< Sim::Bot >::load(fp);
	}
}
