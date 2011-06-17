#ifndef SIM_WORLD_H
#define SIM_WORLD_H

#include <stdint.h>
#include <vector>

namespace Sim {
	class Tile {
		public:
			
		private:
			
	};
	
	class World {
		public:
			World(uint32_t width, uint32_t height);
			~World();
			
			Tile *getTile(uint32_t xInd, uint32_t yInd);
			
			
		private:
			uint32_t mWidth, mHeight;
			
			typedef std::vector<Tile*> TileVec;
			
			TileVec mData;
			Tile *mOffScreen;
	};
}

#endif
