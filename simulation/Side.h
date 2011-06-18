#ifndef SIM_SIDE_H
#define SIM_SIDE_H

#include <stdint.h>
#include <vector>

namespace Sim {
	struct Side {
		uint32_t sideId;
		uint32_t allyGroup;
		
	};
	
	class SideData {
		public:
			SideData();
			~SideData();
			
			void startup();
			void shutdown();
			
			void addSide(Side &side);
			const Side &getSide(uint32_t id) const
			{ return mData[id]; }
			
		private:
			typedef std::vector<Side> SideVec;
			
			SideVec mData;
	};
	
}

#endif
