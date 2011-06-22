#include "Bot.h"

namespace Sim {
	void Bot::handleInput()
	{
		if(isIdle() && mInput.hasInput())
			mCurInput = mInput.nextInput();
		
		if(!isIdle()) {
			switch(mCurInput.type)
			{
				case BotInput::Move:
					mBody.addMomentum( mCurInput.dir );
					break;
				
				case BotInput::Shoot:
					break;
					
				case BotInput::Ability:
					break;
			}
			
			mCurInput.stepCount--;
		}
	}
}