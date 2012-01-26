#ifndef EXTSIM_EXTSIM_H
#define EXTSIM_EXTSIM_H

#include "../simulation/Simulation.h"

#include "ExtData.h"

namespace ExtS {
	/**
	 * @brief Manages simulation data not directly related to simulation.
	 * 
	 * In essence this class implements the "game" on top of the simulation.
	 * 
	 * Provides a system to load data files, define players and bots,
	 * simulation input passing, and aid in game management.
	 * 
	 * The core simulation is loosely protected inside the extended simulation,
	 * and any access to it should generally be limited to reading. Other
	 * interaction should be done through this class.
	 */
	class ExtSim {
		public:
			/// @name Initialization
			//@{
				ExtSim();
				~ExtSim();
			//@}
			
			/// @name Module accessors
			//@{
				ExtData &getData() { return mData; }
				
				Sim::Simulation &getSim() { return mSim; }
			//@}
			
		private:
			Sim::Simulation mSim;
			
			/// @name Subsystems
			//@{
				ExtData mData;
			//@}
	};
}

#endif
