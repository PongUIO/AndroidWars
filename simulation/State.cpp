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
		mAbilityFactory(sim),
		mBulletFactory(sim),
		mWeaponFactory(sim),
		mWorld(sim),
		mPlayer(),
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
		registerCallObj(&mPlayer);
		registerCallObj(&mAbilityFactory);
	}
	
	void State::startup()
	{
		curPhase() = 0;
		curPhaseStep() = 0;
		
		call( boost::bind(&StateObj::startup, _1) );
	}

	void State::shutdown()
	{	rcall( boost::bind(&StateObj::shutdown, _1) ); }
	
	void State::startPhase()
	{
		curPhaseStep() = 0;
		
		call( boost::bind(&StateObj::startPhase, _1) );
	}
	
	void State::step(double stepTime)
	{
		call( boost::bind(&StateObj::step, _1, stepTime) );
		
		curPhaseStep()++;
	}
	
	void State::endPhase()
	{
		call( boost::bind(&StateObj::endPhase, _1) );
		
		curPhase()++;
		curPhaseStep() = 0;
	}
	
	void State::save(Save::BasePtr &fp)
	{
		fp << mCtrl;
		
		call( boost::bind(&StateObj::save, _1, boost::ref(fp)) );
	}
	
	void State::load(Save::BasePtr& fp)
	{
		fp >> mCtrl;
		
		call( boost::bind(&StateObj::load, _1, boost::ref(fp)) );
	}
}
