#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <stdint.h>

#include "Input.h"
#include "Bot.h"
#include "World.h"

namespace Sim {
	struct Configuration {
		uint32_t phaseLength;  ///< Number of steps in a simulation phase
		double stepTime;       ///< Duration of a single simulation step
	};
	
	class Simulation {
		public:
			/// @name Initialization
			//@{
				Simulation();
				~Simulation();
				
				void startup(const Configuration &config);
				void shutdown();
			//@}
			
			/// @name Phase interface
			//@{
				void startPhase();
				void step();
				void endPhase();
				
				bool hasPhaseStep()
				{ return mCurPhaseStep<config.phaseLength; }
			//@}
			
			/// @name Module accessors
			//@{
				BotFactory &getBotFactory()
				{ return mBotFactory; }
				
				World &getWorld()
				{ return mWorld; }
				
				InputBuffer<BotInput> &getBotInput()
				{ return mBotFactory.getInput(); }
			//@}
			
		private:
			/// @name Subsystems
			//@{
				BotFactory mBotFactory;
				World mWorld;
			//@}
			
			/// @name Phase variables
			//@{
				uint32_t mCurPhaseStep;
			//@}
			
			/// @name Other variables
			//@{
				Configuration config;
			//@}
	};
}

#endif
