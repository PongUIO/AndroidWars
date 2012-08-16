#ifndef EXTSIM_EXTDAMAGE_H
#define EXTSIM_EXTDAMAGE_H

#include "../ExtDataComponent.h"

namespace exts {
	class ExtSim;
	
	class ExtDamageData : public ExtDataComponent {
		public:
			ExtDamageData(ExtSim& esim);
			virtual ~ExtDamageData();
			
			void startup() {}
			void shutdown() {}

			void loadNode(const Nepeta::Node& node);
			void postProcess();
			
		private:
			ExtSim &mExtSim;
	};
}

#endif
