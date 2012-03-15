#ifndef EXTSIM_WEAPON_H
#define EXTSIM_WEAPON_H

#include "../ExtBaseData.h"

namespace ExtS {
	class ExtWeaponData : public ExtBaseData {
		public:
			ExtWeaponData(Sim::Simulation& sim);
			virtual ~ExtWeaponData();
			
			void loadBlock(Script::Block& block);
			void postProcess() {}
			
		private:
	};
}

#endif
