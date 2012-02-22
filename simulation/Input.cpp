#include "Input.h"

#include "Program.h"
#include "Bot.h"
#include "Simulation.h"
#include "State.h"

namespace Sim {
#define _SIM_X(type, name) \
	template<> type &Input::getComponent() { return get##name(); }
	_SIM_X_INPUT_COMPONENTS
#undef _SIM_X
	
	// Input
	//
	//
	Input::Input(Simulation* sim) : mSim(sim),
		mBotInput(sim),
		mProgramInput(sim),
		mAbilityInput(sim),
		mBulletInput(sim),
		mWeaponInput(sim),
		mCpuInput(sim)
	{
		registerCallObj(&mBotInput);
		registerCallObj(&mProgramInput);
		registerCallObj(&mAbilityInput);
		registerCallObj(&mBulletInput);
		registerCallObj(&mWeaponInput);
		registerCallObj(&mCpuInput);
	}
	
	Input::~Input()
	{}
	
	void Input::startup()
	{ call( boost::bind(&InputObj::startup, _1) ); }
	
	void Input::shutdown()
	{ call( boost::bind(&InputObj::shutdown, _1) ); }
	
	void Input::finalizeInput()
	{ call( boost::bind(&InputObj::finalizeInput, _1) ); }
	
	void Input::dispatchInput()
	{ call( boost::bind(&InputObj::dispatchInput, _1) ); }
	
	void Input::save(Save::BasePtr& fp)
	{ call( boost::bind(&InputObj::save, _1, boost::ref(fp)) ); }
	
	void Input::load(Save::BasePtr& fp)
	{ call( boost::bind(&InputObj::load, _1, boost::ref(fp)) ); }
	
	
	// BotCpuInput
	//
	//
	void BotCpuInput::startup()
	{}
	
	void BotCpuInput::shutdown()
	{}
	
	void BotCpuInput::dispatchInput()
	{
		BotFactory &botFact = mSim->getState().getBotFactory();
		
		while(mBuffer.hasInput()) {
			CpuRef cpuRef = mBuffer.nextInput();
			
			Bot *bot = botFact.getBot(cpuRef.mTargetBot);
			if(bot) {
				bot->getState().mCpu.scheduleProgram(
				cpuRef.mProgramId,cpuRef.mDelay);
			}
		}
	}
}
