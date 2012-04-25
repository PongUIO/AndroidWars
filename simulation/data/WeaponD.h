#ifndef SIM_WEAPOND_H
#define SIM_WEAPOND_H

#include "BaseData.h"

namespace Sim {
	class Weapon;
	
	class WeaponDatabase : public DefaultBehaviourDatabase<Weapon> {
		public:
			WeaponDatabase() {}
			virtual ~WeaponDatabase() {}
			
		private:
			
	};
}

#endif
