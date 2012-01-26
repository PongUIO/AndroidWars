#ifndef EXTSIM_WEAPON_H
#define EXTSIM_WEAPON_H

#include "../BaseData.h"

namespace ExtS {
	class WeaponData : public BaseData {
		public:
			WeaponData(Sim::Simulation& sim);
			virtual ~WeaponData();
			
			void loadBlock(Script::Block& block);
			void postProcess() {}
			
		private:
	};
}

#endif
