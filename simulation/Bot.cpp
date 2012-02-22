#include "Bot.h"
#include "Weapon.h"
#include "World.h"
#include "State.h"
#include "Simulation.h"

namespace Sim {
	// Bot
	//
	//
	Bot::Bot(Simulation* sim, IdType id, const Sim::Bot::State& cfg) :
		mId(id), mSim(sim), mDoRemove(false), mState(cfg)
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
		sensor.mTargetBot = NoId;
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
	
	void Bot::checkDeath()
	{
		if(mState.mHealth.isDead()) {
			mState.mAbility.shutdown();
			
			mDoRemove = true;
		}
	}
	
	const Sim::BotD* Bot::getTypePtr() const
	{
		return mSim->getData().getBotDb().getDataById(mState.mType);
	}
	
	Sim::Player* Bot::getPlayerPtr() const
	{
		return &mSim->getState().getPlayerData().getPlayer(mState.mSide);
	}
	
	void Bot::save(Save::BasePtr& fp) const
	{	fp << mState; }

	void Bot::load(Save::BasePtr& fp)
	{	fp >> mState; }
	
	void Bot::State::save(Save::BasePtr &fp) const
	{
		// Base variables
		fp << mSide << mType;
		fp << mSensor << mBody << mHealth;
		
		// Components
		fp << mWeapon << mCpu << mAbility;
	}
	
	void Bot::State::load(Save::BasePtr& fp)
	{
		// Base variables
		fp >> mSide >> mType;
		fp >> mSensor >> mBody >> mHealth;
		
		// Components
		fp >> mWeapon >> mCpu >> mAbility;
	}

	
	// BotFactory
	//
	//
	BotFactory::BotFactory(Simulation *sim)
		: DefaultUidFactory<Bot>(sim)
	{}

	BotFactory::~BotFactory()
	{}
	
	BotFactory& BotFactory::getFactory(Simulation* sim)
	{	return sim->getState().getBotFactory(); }

	
	void BotFactory::startup()
	{
		DefaultUidFactory<Bot>::startup();
	}
	
	void BotFactory::shutdown()
	{
		DefaultUidFactory<Bot>::shutdown();
	}
	
	IdType BotFactory::createBot(const Bot::Config &cfg)
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
		
		factoryCall(boost::bind(&Bot::checkDeath, _1));
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

	Bot* BotFactory::loadObj(IdType id, Save::BasePtr& fp)
	{
		Bot *bot = new Bot(mSim, id);
		bot->load(fp);
		return bot;
	}
}
