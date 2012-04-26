#include <assert.h>

#include "State.h"
#include "Bot.h"
#include "World.h"
#include "Player.h"
#include "Simulation.h"
#include "Save.h"

namespace Sim {
#define _SIM_X(type) \
	template<> type &State::getComponent() \
	{ return get##type(); }
	_SIM_X_STATE_COMPONENTS
#undef _SIM_X
	
	State::State(Simulation* sim) :
		mBotFactory(sim),
		mProgramFactory(sim),
		mAbilityFactory(sim),
		mBulletFactory(sim),
		mWeaponFactory(sim),
		mWorld(sim),
		mPlayerData(),
		mSim(sim)
	{
		registerStateObj();
	}
	
	State::~State()
	{}
	
	void State::operator=(const Sim::State& other)
	{
		mCtrl = other.mCtrl;
		registerStateObj();
	}
	
	void State::registerStateObj()
	{
		CallGroup<StateObj>::clear();
		registerCallObj(&mWorld);
		registerCallObj(&mBotFactory);
		registerCallObj(&mProgramFactory);
		registerCallObj(&mWeaponFactory);
		registerCallObj(&mBulletFactory);
		registerCallObj(&mPlayerData);
		registerCallObj(&mAbilityFactory);
	}
	
	void State::startup(uint32_t phaseLen)
	{
		assert(curState() == StUninitialized &&
			"Invalid state for call");
		
		phaseLength() = phaseLen;
		
		curPhase() = 0;
		curPhaseStep() = 0;
		curState() = StIdle;
		
		call( boost::bind(&StateObj::startup, _1) );
	}

	void State::shutdown()
	{
		assert(curState() == StIdle &&
			"Invalid state for call");
		
		rcall( boost::bind(&StateObj::shutdown, _1) );
		
		curState() = StUninitialized;
		
	}
	
	void State::startPhase()
	{
		assert(curState() == StIdle &&
			"Invalid state for call");
		
		curPhaseStep() = 0;
		curState() = StInPhase;
		
		call( boost::bind(&StateObj::startPhase, _1) );
	}
	
	void State::step(double stepTime)
	{
		assert(curState() == StInPhase &&
			"Invalid state for call");
		
		call( boost::bind(&StateObj::step, _1, stepTime) );
		
		curPhaseStep()++;
		
		if(curPhaseStep() >= phaseLength())
			curState() = StEndPhase;
	}
	
	void State::endPhase()
	{
		assert(curState() == StEndPhase &&
			"Invalid state for call");
		
		call( boost::bind(&StateObj::endPhase, _1) );
		
		curPhase()++;
		curPhaseStep() = 0;
		
		curState() = StIdle;
	}
	
	void State::save(Save::BasePtr &fp)
	{
		assert(curState() == StIdle &&
			"Invalid state for call");
		
		fp << mCtrl;
		
		call( boost::bind(&StateObj::save, _1, boost::ref(fp)) );
	}
	
	void State::load(Save::BasePtr& fp)
	{
		fp >> mCtrl;
		
		call( boost::bind(&StateObj::load, _1, boost::ref(fp)) );
	}
}
