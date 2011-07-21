#include "State.h"
#include "Bot.h"
#include "World.h"
#include "Player.h"
#include "Simulation.h"
#include "Save.h"

namespace Sim {
	State::State(Simulation* sim) :
		mBotFactory(sim),
		mWorld(sim),
		mPlayer(),
		mSim(sim)
	{
		registerCallObj(&mBotFactory);
		registerCallObj(&mWorld);
		registerCallObj(&mPlayer);
	}
	
	State::~State()
	{}
	
	void State::startup()
	{
		call(&StateObj::startup);
	}

	void State::shutdown()
	{
		rcall(&StateObj::shutdown);
	}
	
	void State::startPhase()
	{
		mBotFactory.startPhase();
		mWorld.startPhase();
	}
	
	void State::step(double stepTime)
	{
		callArg(&StateObj::step, stepTime);
	}
	
	void State::endPhase()
	{
		mWorld.endPhase();
		mBotFactory.endPhase();
	}
	
	void State::copyState(Sim::State& other)
	{
		mPlayer.copyState(other);
		mWorld.copyState(other);
		mBotFactory.copyState(other);
	}
	
	void State::save(Save::BasePtr &fp)
	{
		mPlayer.save(fp);
		mWorld.save(fp);
		mBotFactory.save(fp);
	}
}
