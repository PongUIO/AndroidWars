#ifndef SIM_PROGRAMD_H
#define SIM_PROGRAMD_H

#include "BaseData.h"

namespace Sim {
	class Program;
	
	class ProgramDatabase : public DataBehaviourT<Program> {
		public:
			ProgramDatabase() {}
			virtual ~ProgramDatabase() {}
			
			void registerBase();
			void registerAbility();
			void registerAllDefault();
	};
}

#endif
