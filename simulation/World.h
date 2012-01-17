#ifndef SIM_WORLD_H
#define SIM_WORLD_H

#include <stdint.h>
#include <math.h>
#include <vector>

#include "collision/Collision.h"
#include "collision/TileCol.h"

#include "StateObj.h"

#include "Save.h"

namespace Sim {
	class Simulation;
	class World;
	class Sync;
	class State;
	
	class Tile : private Save::OperatorImpl<Tile> {
		public:
			enum Flag {
				/// Determines if this tile is deep inside the terrain.
				Hidden = 0,
			};
			
			Tile(uint16_t type=0);
			~Tile();
			
			void setFlag(Flag flag, bool val)
			{	mFlags = (mFlags & (~(1<<flag))) | (val*(1<<flag)); }
			
			void setType(uint16_t type)
			{	mType = type; }
			
			void setColType(const TileCol::TileType &type)
			{	mColType = type; }
			
			uint16_t getType() const { return mType; }
			TileCol::TileType getColType() const { return mColType; }
			
			void save(Save::BasePtr &fp) const
			{ fp << mType << mColType.data; }
			
			void load(Save::BasePtr &fp)
			{ fp >> mType >> mColType.data; }
			
		private:
			uint16_t mType;
			TileCol::TileType mColType;
			uint8_t mFlags;
			
			friend class World;
	};
	
	class World : public StateObj {
		public:
			World(Simulation *sim);
			~World();
			
			/// @name StateObj calls
			//@{
				void startup();
				void shutdown();
				
				void startPhase();
				void step(double stepTime);
				void endPhase();
				
				void save(Save::BasePtr &fp);
				void load(Save::BasePtr &fp);
			//@}
			
			/// @name Tile information
			//@{
				Tile &getTile(uint32_t xInd, uint32_t yInd);
				
				uint32_t getWidth() { return mWidth; }
				uint32_t getHeight() { return mHeight; }
			//@}
			
			/// @name Collision detection
			//@{
				struct ColResult {
					Collision::Result colRes;
				};
				
				ColResult collide(
					const Vector &pos,
					const Vector &vel,
					const Collision *colObj);
			//@}
			
		private:
			int32_t getIndex(double val)
			{ return int32_t(floor(val*mTileSize)); }
			double getCoord(int32_t index)
			{ return double(index)*mTileSize; }
			
			uint32_t mWidth, mHeight;
			double mTileSize;
			
			typedef std::vector<Tile> TileVec;
			
			TileVec mData;
			Tile mOffScreen;
			
			Simulation *mSim;
	};
}

#endif
