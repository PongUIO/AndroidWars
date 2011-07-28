#ifndef SIM_STATE_H
#define SIM_STATE_H

#include <stdint.h>

#include "World.h"
#include "Bot.h"
#include "Bullet.h"
#include "Player.h"

#include "utility/CallGroup.h"
#include "StateObj.h"

namespace Sim {
	class Simulation;
	
	class State : private CallGroup<StateObj> {
		public:
			/// @name Initialization
			//@{
				State(Simulation *sim);
				~State();
				
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
				BotFactory &getBotFactory()
				{ return mBotFactory; }
				
				BulletFactory &getBulletFactory()
				{ return mBulletFactory; }
				
				World &getWorld()
				{ return mWorld; }
				
				PlayerData &getPlayerData()
				{ return mPlayer; }
			//@}
			
		private:
			BotFactory mBotFactory;
			BulletFactory mBulletFactory;
			World mWorld;
			PlayerData mPlayer;
			
			Simulation *mSim;
	};
}

#endif
