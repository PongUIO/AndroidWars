#ifndef SIM_TILECOL_H
#define SIM_TILECOL_H

#include <boost/unordered_map.hpp>

#include "../data/BaseData.h"

namespace Sim {
	// Forward declarations
	class Collision;
	class Simulation;
	
	/**
	 * This class contains an index of tile collision objects.
	 */
	class TileCol : public BaseDatabase {
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
				
				enum BitCount {
					SideBits = 2,
					SlopeBits = 3
				};
				
				enum BitOffset {
					SideBO = 0,
					SlopeLBO = SideBits,
					SlopeRBO = SideBits+SlopeBits
				};
				
				enum BitMask {
					SideBM   = 0x03,
					SlopeLBM = 0x1C,
					SlopeRBM = 0xE0,
				};
				
				TileType(SideType side, uint8_t a, uint8_t b)
					{
						data  = (side<<SideBO)   & SideBM;
						data |= (a   <<SlopeLBO) & SlopeLBM;
						data |= (b   <<SlopeRBO) & SlopeRBM; 
					}
				
				uint8_t getSide()   const
				{ return (data&SideBM)   >> SideBO; }
				uint8_t getSlopeL() const
				{ return (data&SlopeLBM) >> SlopeLBO; }
				uint8_t getSlopeR() const
				{ return (data&SlopeRBM) >> SlopeRBO; }
				
				uint8_t data;
			};
			
			TileCol();
			~TileCol();
			
			void startup(Simulation *sim);
			void shutdown();
			
			Collision *getTileCol(const TileType &type);
			
		private:
			Collision *generateTile(const TileType &type);
			
			double getSlope(uint32_t sy);
			double getTileSize();
			
			typedef boost::unordered_map<TileType, Collision*> TileMap;
			
			TileMap mData;
			Simulation *mSim;
			
			 /// Resolution of possible tile slopes
			static const uint32_t sSlopeRes = 1<<TileType::SlopeBits;
	};
	
	/// Hash function for TileCol::TileType
	inline std::size_t hash_value(const TileCol::TileType &val)
	{	return boost::hash_value(val.data); }
}

#endif
