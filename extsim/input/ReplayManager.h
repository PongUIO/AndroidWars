#ifndef EXTSIM_REPLAYMANAGER_H
#define EXTSIM_REPLAYMANAGER_H

namespace ExtS {
	class ExtSim;
	
	/**
	 * @brief
	 */
	class ReplayManager {
		public:
			ReplayManager(ExtSim &esim);
			~ReplayManager();
			
		private:
			ExtSim *mExtSim;
	};
}

#endif
