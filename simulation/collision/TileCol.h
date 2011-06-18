#ifndef SIM_TILECOL_H
#define SIM_TILECOL_H

#include <boost/unordered_map.hpp>

namespace Sim {
	// Forward declarations
	class Collision;
	
	/**
	 * This class contains an index of tile collision objects.
	 */
	class TileCol {
		public:
			struct TileType {
				/**
				 * Defines the indexes for each base side.
				 */
				enum SideType {
					Left	= 0,
					Up		= 1,
					Right	= 2,
					Down	= 3,
					
					MaxSide
				};
				
				TileType(SideType side, uint8_t a, uint8_t b)
					: side(side)
					{
						sl[0] = a;
						sl[1] = b;
					}
				
				uint8_t side;
				uint8_t sl[2];
			};
			
			TileCol();
			~TileCol();
			
			void startup(double tileSize, uint32_t slopeRes);
			void shutdown();
			
			Collision *getTileCol(const TileType &type);
			
		private:
			Collision *generateTile(const TileType &type);
			double getSlope(uint32_t sy)
			{ return (1.0-double(sy)/double(mSlopeRes))*mTileSize; }
			
			typedef boost::unordered_map<TileType, Collision*> TileMap;
			
			TileMap mData;
			double mTileSize;
			uint32_t mSlopeRes;    ///< Resolution of possible tile slopes
	};
}

#endif
