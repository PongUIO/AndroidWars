#ifndef EXTSIM_EXTCPUINPUT_H
#define EXTSIM_EXTCPUINPUT_H

#include "../../simulation/Common.h"

namespace exts {
	class ExtSim;
	namespace misc {
		class CpuInput;
	}
	
	/**
	 * @brief Minor class to manage CPU input to simulation bots
	 */
	class ExtCpuInput {
		public:
			ExtCpuInput(ExtSim &esim);
			~ExtCpuInput();
			
			void startup();
			void shutdown();
			
			void registerCpuInput(Sim::IdType bot, Sim::IdType prog,
				uint32_t delay);
			
		private:
			ExtSim &mExtSim;
			
			misc::CpuInput *mCpuInput;
	};
}

#endif
