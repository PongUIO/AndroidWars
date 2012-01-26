#ifndef TM_TIMELINEMGR_H
#define TM_TIMELINEMGR_H

#include "../simulation/Simulation.h"

namespace Tm {
	/**
	 * @brief Helper class to manage simulation input and events.
	 * 
	 * The timeline manager is designed to simplify the planning phase
	 * by aiding in creating input, visualizing input and simulation events,
	 * and manage replays.
	 */
	class TimelineMgr {
		public:
			TimelineMgr(Sim::Simulation &sim) : mSim(sim) {}
			~TimelineMgr() {}
			
		private:
			Sim::Simulation &mSim;
	};
}

#endif
