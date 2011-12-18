#include "Input.h"

#include "Program.h"
#include "Bot.h"
#include "Simulation.h"
#include "State.h"

namespace Sim {
	// BotInput
	//
	//
	void BotInput::save(Save::BasePtr& fp)
	{
		fp.writeInt<uint32_t>(mTargetBot);
		fp.writeInt<uint32_t>(mProgramId);
		fp.writeInt<uint32_t>(mDelay);
	}
	
	void BotInput::load(Save::BasePtr& fp)
	{
		mTargetBot = fp.readInt<uint32_t>();
		mProgramId = fp.readInt<uint32_t>();
		mDelay = fp.readInt<uint32_t>();
	}
	
	// InputManager
	//
	//
	InputManager::InputManager(Simulation* sim) : mSim(sim), mBotInput()
	{}
	
	InputManager::~InputManager()
	{}
	
	void InputManager::startup()
	{}
	
	void InputManager::shutdown()
	{}
	
	void InputManager::startPhase()
	{
		BotFactory &botFact = mSim->getState().getBotFactory();
		
		while(mBotInput.hasInput()) {
			BotInput in = mBotInput.nextInput();
			
			Bot *bot = botFact.getBot(in.mTargetBot);
			if(bot)
				bot->getState().mCpu.scheduleProgram(in.mProgramId,in.mDelay);
		}
	}
	
	void InputManager::endPhase()
	{}
	
	void InputManager::step(double stepTime)
	{}
	
	void InputManager::save(Save::BasePtr& fp)
	{
		mBotInput.save(fp);
	}

	void InputManager::load(Save::BasePtr& fp)
	{
		mBotInput.load(fp);
	}

}
