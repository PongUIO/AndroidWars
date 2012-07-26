#ifndef EXTSIM_EXTMAP_H
#define EXTSIM_EXTMAP_H

#include <stdint.h>

#include "../ExtDataComponent.h"

namespace ExtS {
	class ExtSim;
	
	class ExtMapData : public ExtDataComponent {
		public:
			ExtMapData(ExtSim& sim);
			virtual ~ExtMapData();
			
			void startup();
			void shutdown();
			
			void loadNode(const Nepeta::Node& node);
			void postProcess();
			
		private:
			ExtSim &mExtSim;
	};
}

#endif
