#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <stdint.h>

#include "CommonTemplate.h"

#include "Data.h"
#include "State.h"
#include "Input.h"
#include "Replay.h"
#include "program/ProgramInclude.h"
#include "ability/AbilityInclude.h"

#include "Common.h"

/**
 * @namespace Sim
 * 
 * @brief Core simulation namespace.
 * 
 * The simulation namespace contains everything 
 */
namespace Sim {
	struct Configuration {
		Configuration() :
			phaseLength(25),
			stepTime(1.0),
			
			tileSize(1.0)
			{}
		
		uint32_t phaseLength;  ///< Number of steps in a simulation phase
		double stepTime;       ///< Duration of a single simulation step
		
		double tileSize;       ///< Size of each world tile
		//uint32_t worldWidth;   ///< Width of the world in tiles
		//uint32_t worldHeight;  ///< Height of the world in tiles
	};
	
#define _SIM_X_SIMULATION_COMPONENTS \
	_SIM_X(State) \
	_SIM_X(Input) \
	_SIM_X(Data) \
	_SIM_X(ReplayManager) \
	_SIM_X(Configuration)
	
	class Simulation {
		public:
			/// @name Initialization
			//@{
				Simulation();
				~Simulation();
				
				void startup();
				void shutdown();
				
				void clear();
			//@}
			
			/// @name Phase/simulation interface
			//@{
				void prepareSim();
				
				void startPhase();
				void step();
				void endPhase(bool finalize);
				
				void gotoPresent() { mReplayManager.gotoPresent(); }
				
				bool hasPhaseStep()
				{ return getState().getStateType() == State::StInPhase; }
				
				uint32_t getCurPhase()
				{ return getState().getCurPhase(); }
				
				uint32_t getCurPhaseStep()
				{ return getState().getCurPhaseStep(); }
				
				uint32_t getCurTotalStep()
				{ return getCurPhase()*getConfiguration().phaseLength+getCurPhaseStep(); }
				
				double getCurTime()
				{ return double(getCurTotalStep())*getConfiguration().stepTime; }
			//@}
			
			/// @name Module accessors
			//@{
				template<class T>
				T &getComponent();
				
#define _SIM_X(type) type &get##type() { return m##type; }
				_SIM_X_SIMULATION_COMPONENTS
#undef _SIM_X
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
#define _SIM_X(type) type m##type;
				_SIM_X_SIMULATION_COMPONENTS
#undef _SIM_X
			//@}
	};
}

#endif
