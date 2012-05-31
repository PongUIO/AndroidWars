#ifndef EXTSIM_EXTMAP_H
#define EXTSIM_EXTMAP_H

#include <stdint.h>

#include "../ExtBaseData.h"

namespace ExtS {
	class ExtMapData : public ExtBaseData {
		public:
			ExtMapData(ExtSim& sim);
			virtual ~ExtMapData();
			
			void startup();
			void shutdown();
			
			void loadNode(Nepeta::Node& node);
			
			void postProcess();
			
		private:
	};
}

#endif
