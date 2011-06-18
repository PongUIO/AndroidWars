#ifndef SIM_WORLD_H
#define SIM_WORLD_H

#include <stdint.h>
#include <vector>

#include "collision/TileCol.h"

namespace Sim {
	class Simulation;
	
	class Tile {
		public:
			
		
		private:
			float mHp;
	};
	
	class World {
		public:
			World(Simulation *sim);
			~World();
			
			Tile *getTile(uint32_t xInd, uint32_t yInd);
			
			void startup();
			void shutdown();
			
		private:
			uint32_t mWidth, mHeight;
			
			typedef std::vector<Tile*> TileVec;
			
			TileVec mData;
			Tile *mOffScreen;
			
			TileCol mTileCol;
			
			Simulation *mSim;
	};
}

#endif
