#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <stdint.h>

#include "Data.h"
#include "State.h"

namespace Sim {
	struct Configuration {
		Configuration() :
			phaseLength(100),
			stepTime(0.01),
			
			tileSize(1.0)
			{}
		
		uint32_t phaseLength;  ///< Number of steps in a simulation phase
		double stepTime;       ///< Duration of a single simulation step
		
		double tileSize;       ///< Size of each world tile
		
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
				
				uint32_t getCurPhase()
				{ return mCurPhase; }
				
				void rewindPhase();
			//@}
			
			/// @name Module accessors
			//@{
				State &getState()
				{ return mStateActive; }
				
				Data &getData()
				{ return mData; }
				
				const Configuration &getConfig()
				{ return config; }
			//@}
			
			/// @name Checksum, saving, and loading
			//@{
				uint32_t checksumData();
				uint32_t checksumSim();
				
				Save save();
				void load(const Save &saveData);
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
				uint32_t mCurPhase;
			//@}
			
			/// @name Other variables
			//@{
				Configuration config;
			//@}
	};
}

#endif
