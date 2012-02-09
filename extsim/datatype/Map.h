#ifndef EXTSIM_MAP_H
#define EXTSIM_MAP_H

#include <stdint.h>

#include "../BaseData.h"

namespace ExtS {
	class MapData : public BaseData {
		public:
			MapData(ExtSim& sim);
			virtual ~MapData();
			
			void startup();
			void shutdown();
			
			void loadBlock(Script::Block& block);
			
			void postProcess();
			
		private:
	};
}

#endif
