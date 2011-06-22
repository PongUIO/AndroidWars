#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <stdint.h>

#include "Data.h"
#include "State.h"

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
			
			/// @name Phase/simulation interface
			//@{
				void prepareSim();
				
				void startPhase();
				void step();
				void endPhase();
				
				bool hasPhaseStep()
				{ return mCurPhaseStep<config.phaseLength; }
				
				void rewindPhase();
				void finalizePhase();
			//@}
			
			/// @name Module accessors
			//@{
				State &getState()
				{ return mStateActive; }
				
				Data &getData()
				{ return mData; }
			//@}
			
			/// @name Checksum
			//@{
				uint32_t checksumData();
				uint32_t checksumSim();
			//@}
			
		private:
			/// @name Subsystems
			//@{
				State mStateActive;
				State mStateCopy;
				Data mData;
				
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
