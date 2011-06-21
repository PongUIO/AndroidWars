#ifndef SIM_STATE_H
#define SIM_STATE_H

#include <stdint.h>

#include "World.h"
#include "Bot.h"
#include "Side.h"

namespace Sim {
	class Simulation;
	
	class State {
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
				void copyState(State &other);
				
				uint32_t checksum();
			//@}
			
			/// @name Module accessors
			//@{
				BotFactory &getBotFactory()
				{ return mBotFactory; }
				
				World &getWorld()
				{ return mWorld; }
				
				SideData &getSideData()
				{ return mSide; }
			//@}
			
		private:
			BotFactory mBotFactory;
			World mWorld;
			SideData mSide;
			
			Simulation *mSim;
	};
}

#endif
