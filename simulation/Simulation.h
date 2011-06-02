#ifndef SIMULATION_H
#define SIMULATION_H

#include "Input.h"

namespace Sim {
	class Simulation {
		public:
			Simulation();
			~Simulation();
			
			void exec();
			
		private:
			InputBuffer mInput;
	};
}

#endif
