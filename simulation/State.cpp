#include "State.h"
#include "Bot.h"
#include "World.h"
#include "Player.h"
#include "Simulation.h"
#include "Save.h"

namespace Sim {
	State::State(Simulation* sim) :
		mBotFactory(sim),
		mBulletFactory(sim),
		mWeaponFactory(sim),
		mWorld(sim),
		mPlayer(),
		mSim(sim)
	{
		registerCallObj(&mBotFactory);
		registerCallObj(&mWeaponFactory);
		registerCallObj(&mBulletFactory);
		registerCallObj(&mWorld);
		registerCallObj(&mPlayer);
	}
	
	State::~State()
	{}
	
	void State::startup()
	{	call( boost::bind(&StateObj::startup, _1) ); }

	void State::shutdown()
	{	rcall( boost::bind(&StateObj::shutdown, _1) ); }
	
	void State::startPhase()
	{
		mBotFactory.startPhase();
		mWorld.startPhase();
	}
	
	void State::step(double stepTime)
	{	call( boost::bind(&StateObj::step, _1, stepTime) ); }
	
	void State::endPhase()
	{
		mWorld.endPhase();
		mBotFactory.endPhase();
	}
	
	void State::save(Save::BasePtr &fp)
	{
		mPlayer.save(fp);
		mWorld.save(fp);
		mBotFactory.save(fp);
	}
	
	void State::load(Save::BasePtr& fp)
	{
	}
}
