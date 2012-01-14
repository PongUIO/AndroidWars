#include "Bot.h"
#include "Weapon.h"
#include "World.h"
#include "State.h"
#include "Simulation.h"

namespace Sim {
	// Bot
	//
	//
	Bot::Bot(Simulation* sim, uint32_t id, const Sim::Bot::State& cfg) :
		mId(id), mSim(sim), mState(cfg)
	{
		mState.mWeapon.initialize(this);
		mState.mAbility.initialize(this);
		mState.mCpu.initialize(this);
		const BotD *data = getTypePtr();
		if(data)
			mState.mHealth = data->coreHealth;
	}

	Bot::~Bot()
	{
	}
	
	void Bot::prepareStep(double stepTime)
	{
		// Update abilties
		mState.mAbility.prepareStep(stepTime);
		
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
		mState.mAbility.updateCpu(stepTime);
	}
	
	void Bot::step(double stepTime)
	{
		mState.mAbility.step(stepTime);
		
		mState.mBody.addMomentum(
			mState.mEngine.mDirection*(mState.mEngine.mStrength*stepTime) );
		
		mState.mBody.step(stepTime);
		
		World &world = mSim->getState().getWorld();
		World::ColResult res;
		res = world.collide(mState.mBody.mPos, mState.mBody.mVel, getTypePtr()->getCollision());
		if(res.colRes.isCol) {
			mState.mBody.mPos += res.colRes.getOrp();
		}
		
		mState.mWeapon.step(stepTime);
	}
	
	const Sim::BotD* Bot::getTypePtr() const
	{
		return mSim->getData().getBotDb().getType(mState.mType);
	}
	
	Sim::Player* Bot::getPlayerPtr() const
	{
		return &mSim->getState().getPlayerData().getPlayer(mState.mSide);
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
		mHealth.save(fp);
		
		mWeapon.save(fp);
		mCpu.save(fp);
		mAbility.save(fp);
	}
	
	void Bot::State::load(Save::BasePtr& fp)
	{
		mSide = fp.readInt<uint32_t>();
		mType = fp.readInt<uint32_t>();
		
		mSensor.load(fp);
		
		mBody.load(fp);
		mHealth.load(fp);
		
		mWeapon.load(fp);
		mCpu.load(fp);
		mAbility.load(fp);
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
		: DefaultUidFactory<Bot>(sim)
	{}

	BotFactory::~BotFactory()
	{}
	
	void BotFactory::startup()
	{
		DefaultUidFactory<Bot>::startup();
	}
	
	void BotFactory::shutdown()
	{
		DefaultUidFactory<Bot>::shutdown();
	}
	
	uint32_t BotFactory::createBot(const Bot::Config &cfg)
	{
		BotFactory::InsertData insData = insertObject();
		
		Bot *bot = new Bot(mSim, insData.first, cfg);
		*insData.second = bot;
		
		return insData.first;
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
	{	DefaultUidFactory<Bot>::save(fp); }

	void Sim::BotFactory::load(Save::BasePtr& fp)
	{	DefaultUidFactory<Bot>::load(fp); }

	
	void BotFactory::saveObj(Bot* bot, Save::BasePtr& fp)
	{	bot->save(fp); }

	Bot* BotFactory::loadObj(uint32_t id, Save::BasePtr& fp)
	{
		Bot *bot = new Bot(mSim, id);
		bot->load(fp);
		return bot;
	}
}
