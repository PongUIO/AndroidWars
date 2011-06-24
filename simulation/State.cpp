#include "State.h"
#include "Bot.h"
#include "World.h"
#include "Side.h"
#include "Simulation.h"
#include "Save.h"

namespace Sim {
	State::State(Simulation* sim) :
		mBotFactory(sim),
		mWorld(sim),
		mSide(),
		mSim(sim)
	{}
	
	State::~State()
	{}
	
	void State::startup()
	{
		mBotFactory.startup();
		mWorld.startup();
		mSide.startup();
	}

	void State::shutdown()
	{
		mSide.shutdown();
		mWorld.shutdown();
		mBotFactory.shutdown();
	}
	
	void State::startPhase()
	{
		mBotFactory.startPhase();
		mWorld.startPhase();
	}
	
	void State::step(double stepTime)
	{
		mBotFactory.step(stepTime);
		mWorld.step(stepTime);
	}
	
	void State::endPhase()
	{
		mWorld.endPhase();
		mBotFactory.endPhase();
	}
	
	void State::copyState(Sim::State& other)
	{
		//mSide.copyState(other);
		mWorld.copyState(other);
		mBotFactory.copyState(other);
	}
	
	void State::save(Save &file)
	{
		Save::Ptr fp = Save::Ptr(file);
		
		mWorld.save(fp);
		mBotFactory.save(fp);
	}
}
