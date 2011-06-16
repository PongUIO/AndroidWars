#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdint.h>

#include "Input.h"
#include "Bot.h"

namespace Sim {
	struct Configuration {
		double phaseTime;    ///< Duration of a simulation phase
		double stepTime;     ///< Duration of a single simulation step
	};
	
	class Simulation {
		public:
			Simulation();
			~Simulation();
			
			void startup(const Configuration &config);
			void shutdown();
			
			void startPhase();
			void step();
			void endPhase();
			
			//Sync checksum();
			
			BotFactory &getBotFactory() {
				return mBotFactory;
			}
			
			InputBuffer<BotInput> &getBotInput() {
				return mBotFactory.getInput();
			}
			
		private:
			/// @name Subsystems
			//@{
				BotFactory mBotFactory;
			//@}
	};
}

#endif
