#ifndef SIM_STATE_H
#define SIM_STATE_H

#include <stdint.h>

#include "World.h"
#include "Bot.h"
#include "Bullet.h"
#include "Weapon.h"
#include "Player.h"
#include "Program.h"
#include "Ability.h"
#include "Input.h"
#include "Replay.h"

#include "utility/CallGroup.h"
#include "StateObj.h"

namespace Sim {
	class Simulation;
	class BotFactory;
	class WeaponFactory;
	class BulletFactory;
	
	class State : private CallGroup<StateObj> {
		public:
			/// @name Initialization
			//@{
				State(Simulation *sim);
				~State();
				
				void operator=(const State &other);
				
				void startup();
				void shutdown();
			//@}
			
			/// @name Phase interface
			//@{
				void startPhase();
				void step(double stepTime);
				void endPhase();
			//@}
			
			/// @name State interface
			//@{
				void save(Save::BasePtr &fp);
				void load(Save::BasePtr &fp);
			//@}
			
			/// @name Module accessors
			//@{
				InputManager &getInputManager()
				{ return mInputManager; }
				
				BotFactory &getBotFactory()
				{ return mBotFactory; }
				
				ProgramFactory &getProgramFactory()
				{ return mProgramFactory; }
				
				AbilityFactory &getAbilityFactory()
				{ return mAbilityFactory; }
				
				BulletFactory &getBulletFactory()
				{ return mBulletFactory; }
				
				WeaponFactory &getWeaponFactory()
				{ return mWeaponFactory; }
				
				World &getWorld()
				{ return mWorld; }
				
				PlayerData &getPlayerData()
				{ return mPlayer; }
				
				uint32_t getCurPhaseStep() { return mCtrl.mCurPhaseStep; }
				uint32_t getCurPhase() { return mCtrl.mCurPhase; }
			//@}
			
		private:
			/// @name State objects
			//@{
				void registerStateObj();
				
				InputManager mInputManager;
				BotFactory mBotFactory;
				ProgramFactory mProgramFactory;
				AbilityFactory mAbilityFactory;
				BulletFactory mBulletFactory;
				WeaponFactory mWeaponFactory;
				World mWorld;
				PlayerData mPlayer;
			//@}
			
			/// @name State control
			//@{
				uint32_t &curPhaseStep() { return mCtrl.mCurPhaseStep; }
				uint32_t &curPhase() { return mCtrl.mCurPhase; }
				
				struct StateControl :
					private Save::OperatorImpl<StateControl> {
					uint32_t mCurPhaseStep;
					uint32_t mCurPhase;
					
					void save(Save::BasePtr &fp) const
					{ fp << mCurPhaseStep << mCurPhase; }
					void load(Save::BasePtr &fp)
					{ fp >> mCurPhaseStep >> mCurPhase; }
				};
				StateControl mCtrl;
			//@}
			
			Simulation *mSim;
	};
}

#endif
