#include "State.h"
#include "Bot.h"
#include "World.h"
#include "Player.h"
#include "Simulation.h"
#include "Save.h"

namespace Sim {
	State::State(Simulation* sim) :
		mBotFactory(sim),
		mProgramFactory(sim),
		mBulletFactory(sim),
		mWeaponFactory(sim),
		mWorld(sim),
		mPlayer(),
		mSim(sim)
	{
		registerCallObj(&mWorld);
		registerCallObj(&mBotFactory);
		registerCallObj(&mProgramFactory);
		registerCallObj(&mWeaponFactory);
		registerCallObj(&mBulletFactory);
		registerCallObj(&mPlayer);
	}
	
	State::~State()
	{}
	
	void State::startup()
	{	call( boost::bind(&StateObj::startup, _1) ); }

	void State::shutdown()
	{	rcall( boost::bind(&StateObj::shutdown, _1) ); }
	
	void State::startPhase()
	{	call( boost::bind(&StateObj::startPhase, _1) ); }
	
	void State::step(double stepTime)
	{	call( boost::bind(&StateObj::step, _1, stepTime) ); }
	
	void State::endPhase()
	{	call( boost::bind(&StateObj::endPhase, _1) );	}
	
	void State::save(Save::BasePtr &fp)
	{	call( boost::bind(&StateObj::save, _1, boost::ref(fp)) );	}
	
	void State::load(Save::BasePtr& fp)
	{	call( boost::bind(&StateObj::load, _1, boost::ref(fp)) ); }
}
