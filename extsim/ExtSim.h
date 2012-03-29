#ifndef EXTSIM_EXTSIM_H
#define EXTSIM_EXTSIM_H

#include "../simulation/Simulation.h"

#include "ExtData.h"
#include "ExtInput.h"

namespace ExtS {
#define _EXTS_X_EXTSIM_COMPONENTS \
	_EXTS_X(Sim::Simulation, Sim) \
	_EXTS_X(ExtData, Data) \
	_EXTS_X(ExtInput, Input)
	
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
				
				void startup();
				void shutdown();
				
				void loadDataScript(const std::string &script)
				{ mData.loadScript(script); }
				void postProcessData()
				{ mData.postProcess(); }
				void switchDataContext(ExtData::ListenerContext context)
				{ mData.switchContext(context); }
				
				void prepareSim();
				
				void reset();
			//@}
			
			/// @name Module accessors
			//@{
				template<class T>
				T &getComponent();
				
#define _EXTS_X(type, name) type &get##name() { return m##name; }
				_EXTS_X_EXTSIM_COMPONENTS
#undef _EXTS_X
			//@}
			
		private:
			/// @name Subsystems
			//@{
#define _EXTS_X(type, name) type m##name;
				_EXTS_X_EXTSIM_COMPONENTS
#undef _EXTS_X
			//@}
	};
}

#endif
