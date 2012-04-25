#ifndef SIM_PROGRAMD_H
#define SIM_PROGRAMD_H

#include "BaseData.h"

namespace Sim {
	class Program;
	
	class ProgramDatabase : public DefaultBehaviourDatabase<Program> {
		public:
			ProgramDatabase() {}
			virtual ~ProgramDatabase() {}
	};
}

#endif
