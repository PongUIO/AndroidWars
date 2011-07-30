#include "Simulation.h"
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
				{
					Weapon *w = mSim->getState().getWeaponFactory().getObj(mWeaponBox.get(0));
					Weapon::ShootArg sa;
					sa.pos = mBody.mPos;
					sa.dir = Vector(1,0);
					w->shoot(sa, 0);
				}
					break;
					
				case BotInput::Ability:
					break;
			}
			
			mCurInput.stepCount--;
		}
	}
}