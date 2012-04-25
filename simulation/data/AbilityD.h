#ifndef SIM_ABILITYD_H
#define SIM_ABILITYD_H

#include "BaseData.h"

namespace Sim {
	class Ability;
	
	class AbilityDatabase : public DefaultBehaviourDatabase<Ability> {
		public:
			AbilityDatabase() {}
			virtual ~AbilityDatabase() {}
	};
}

#endif
