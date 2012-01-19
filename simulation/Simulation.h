#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <stdint.h>

#include "Data.h"
#include "State.h"
#include "Replay.h"
#include "program/ProgramInclude.h"
#include "ability/AbilityInclude.h"

#include "Common.h"

namespace Sim {
	struct Configuration {
		Configuration() :
			phaseLength(100),
			stepTime(0.01),
			
			tileSize(1.0),
			worldWidth(128),
			worldHeight(64)
			{}
		
		uint32_t phaseLength;  ///< Number of steps in a simulation phase
		double stepTime;       ///< Duration of a single simulation step
		
		double tileSize;       ///< Size of each world tile
		uint32_t worldWidth;   ///< Width of the world in tiles
		uint32_t worldHeight;  ///< Height of the world in tiles
	};
	
	class Simulation {
		public:
			/// @name Initialization
			//@{
				Simulation();
				~Simulation();
				
				void startup(const Configuration &config);
				void shutdown();
				
				void clear();
			//@}
			
			/// @name Phase/simulation interface
			//@{
				void prepareSim();
				
				void startPhase();
				void step();
				void endPhase(bool finalize);
				
				void gotoPresent() { mReplay.gotoPresent(); }
				
				bool hasPhaseStep()
				{ return getCurPhaseStep()<config.phaseLength; }
				
				uint32_t getCurPhase()
				{ return getState().getCurPhase(); }
				
				uint32_t getCurPhaseStep()
				{ return getState().getCurPhaseStep(); }
				
				uint32_t getCurTotalStep()
				{ return getCurPhase()*config.phaseLength+getCurPhaseStep(); }
				
				double getCurTime()
				{ return double(getCurTotalStep())*config.stepTime; }
			//@}
			
			/// @name Module accessors
			//@{
				State &getState()
				{ return mStateActive; }
				
				Data &getData()
				{ return mData; }
				
				ReplayManager &getReplayManager()
				{ return mReplay; }
				
				const Configuration &getConfig()
				{ return config; }
			//@}
			
			/// @name Checksum, saving, and loading
			//@{
				uint32_t checksumData();
				uint32_t checksumSim();
				
				Save save();
				void load(Save &saveData);
				
				void save(Save::BasePtr &fp);
			//@}
			
		private:
			/// @name Subsystems
			//@{
				State mStateActive;
				Data mData;
				
				ReplayManager mReplay;
			//@}
			
			/// @name Other variables
			//@{
				Configuration config;
			//@}
	};
}

#endif
