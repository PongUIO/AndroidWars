#ifndef SIM_STATEOBJ_H
#define SIM_STATEOBJ_H

#include "Save.h"

namespace Sim {
	class Simulation;
	class State;
	
	class StateObj {
		public:
			StateObj() {}
			virtual ~StateObj() {}
			
			virtual void startup()=0;
			virtual void shutdown()=0;
			
			virtual void step(double stepTime)=0;
			
			virtual void save(Save::BasePtr &fp) { (void)fp; }
			virtual void load(Save::BasePtr &fp) { (void)fp; }
	};
}

#endif
