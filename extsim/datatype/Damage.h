#ifndef EXTSIM_DAMAGE_H
#define EXTSIM_DAMAGE_H

#include "../BaseData.h"

namespace ExtS {
	class DamageData : public BaseData {
		public:
			DamageData(ExtSim& esim);
			virtual ~DamageData();
			
			void startup() {}
			void shutdown() {}

			void loadBlock(Script::Block& block);
			void postProcess();
	};
}

#endif
