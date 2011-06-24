#ifndef SIM_WORLD_H
#define SIM_WORLD_H

#include <stdint.h>
#include <vector>

#include "collision/TileCol.h"
#include "Save.h"

namespace Sim {
	class Simulation;
	class World;
	class Sync;
	class State;
	
	class Tile {
		public:
			Tile(uint16_t type=0);
			~Tile();
			
			uint16_t mType;
			TileCol::TileType mColType;
			
			friend class World;
	};
	
	class World {
		public:
			World(Simulation *sim);
			~World();
			
			void startup();
			void shutdown();
			
			/// @name Tile information
			//@{
				Tile &getTile(uint32_t xInd, uint32_t yInd);
				
				uint32_t getWidth() { return mWidth; }
				uint32_t getHeight() { return mHeight; }
			//@}
			
			void startPhase();
			void step(double stepTime);
			void endPhase();
			
			/// @name State
			//@{
				void save(Save::Ptr &fp);
				void copyState(State &state);
			//@}
			
		private:
			uint32_t mWidth, mHeight;
			
			typedef std::vector<Tile> TileVec;
			
			TileVec mData;
			Tile mOffScreen;
			
			TileCol mTileCol;
			
			Simulation *mSim;
	};
}

#endif
