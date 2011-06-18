#ifndef SIM_TILED_H
#define SIM_TILED_H

#include <vector>
#include <stdint.h>

namespace Sim {
	struct TileD {
		enum ColMask {
			ColBot =    0x1, // Collides with bots
			ColBullet = 0x2, // Collides with bullets
			ColAll = ~0,     // Collides with everything
			ColNone = 0      // Collides with nothing
		};
		ColMask colMask;     ///< Mask for which types the tile collide with
		
		/// The blast resistance for the tile, if negative the tile is immune
		double blastResist;
		
		double bounce;       ///< The bounciness for the tile
		double friction;     ///< The friction for the tile
	};
	
	class TileDatabase {
		public:
			TileDatabase() {}
			~TileDatabase() {}
			
			void startup(uint16_t numTypes=1)
			{ mData.reserve(numTypes); }
			
			void shutdown()
			{ mData.clear(); }
			
			const TileD &getTile(uint16_t type) const
			{ return mData[type]; }
			
			uint16_t addTile(const TileD &tile)
			{ mData.push_back(tile); return mData.size()-1; }
			
		private:
			typedef std::vector<TileD> TileVec;
			
			TileVec mData;
	};
}

#endif
