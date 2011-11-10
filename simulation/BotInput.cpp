#include <stdio.h>
#include "Simulation.h"
#include "Bot.h"

namespace Sim {
	void Bot::handleInput()
	{
		if(isIdle() && mState.mInput.hasInput())
			mState.mCurInput = mState.mInput.nextInput();
		
		if(!isIdle()) {
			switch(mState.mCurInput.type)
			{
				case BotInput::Move:
					mState.mBody.addMomentum( mState.mCurInput.dir );
					break;
				
				case BotInput::Shoot:
				{
					Weapon *w = mSim->getState().getWeaponFactory().getObj(mState.mWeaponBox.get(0));
					Weapon::ShootArg sa;
					sa.pos = mState.mBody.mPos;
					sa.dir = Vector(1,0);
					w->shoot(sa, 0);
				}
					break;
					
				case BotInput::Ability:
					break;
			}
			
			mState.mCurInput.stepCount--;
		}
	}
}
