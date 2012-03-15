#ifndef EXTSIM_EXTDAMAGE_H
#define EXTSIM_EXTDAMAGE_H

#include "../ExtBaseData.h"

namespace ExtS {
	class ExtDamageData : public ExtBaseData {
		public:
			ExtDamageData(ExtSim& esim);
			virtual ~ExtDamageData();
			
			void startup() {}
			void shutdown() {}

			void loadBlock(Script::Block& block);
			void postProcess();
	};
}

#endif
