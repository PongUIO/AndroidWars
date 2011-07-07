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
			/**
			 * The datatype used to describe a collision tile.
			 * The type is aligned as below:
			 * 2-bits : Base side
			 * 3-bits : Side 1
			 * 3-bits : Side 2
			 */
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
					{
						data  = side   & 0x03;
						data |= (a<<2) & 0x1C;
						data |= (b<<5) & 0xE0; 
					}
				
				uint8_t getSide()   const { return (data&0x03) >> 0; }
				uint8_t getSlopeL() const { return (data&0x1C) >> 2; }
				uint8_t getSlopeR() const { return (data&0xE0) >> 5; }
				
				uint8_t data;
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
	
	/// Hash function for TileCol::TileType
	inline std::size_t hash_value(const TileCol::TileType &val)
	{	return boost::hash_value(val.data); }
}

#endif
