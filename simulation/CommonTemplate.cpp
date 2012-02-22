#include "CommonTemplate.h"

#include "Simulation.h"

namespace Sim {

#define _SIM_X(type) \
	template<> type &getSimulationComponent(Simulation &sim) \
	{ return sim.getComponent<type>(); }
	_SIM_X_SIMULATION_COMPONENTS
#undef _SIM_X

#define _SIM_X(type) \
	template<> type &getStateComponent<type>(Simulation &sim) \
	{ return getSimulationComponent<State>(sim).getComponent<type>(); }
	_SIM_X_STATE_COMPONENTS
#undef _SIM_X

#define _SIM_X(type, name) \
	template<> type &getDataComponent<type>(Simulation &sim) \
	{ return getSimulationComponent<Data>(sim).getDatabase<type>(); }
	_SIM_X_DATA_COMPONENTS
#undef _SIM_X

#define _SIM_X(type, name) \
	template<> type & \
	getInputComponent<type>(Simulation &sim) \
	{ return getSimulationComponent<Input>(sim).getComponent<type>(); }
	_SIM_X_INPUT_COMPONENTS
#undef _SIM_X
}
