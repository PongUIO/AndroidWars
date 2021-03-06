#ifndef SIM_TILED_H
#define SIM_TILED_H

#include <vector>
#include <stdint.h>
#include "../Save.h"

#include "BaseData.h"

namespace Sim {
	struct TileD {
		TileD() :
			colMask(0),
			blastResist(0.0), bounce(0.0), friction(0.0)
			{}
		
		enum ColMask {
			ColBot =    0x1, // Collides with bots
			ColBullet = 0x2, // Collides with bullets
			ColAll = ~0,     // Collides with everything
			ColNone = 0      // Collides with nothing
		};
		uint32_t colMask;     ///< Mask for which types the tile collide with
		
		/// The blast resistance for the tile, if negative the tile is immune
		double blastResist;
		
		double bounce;       ///< The bounciness for the tile
		double friction;     ///< The friction for the tile
	};
	
	class TileDatabase : public DefaultDatabase<TileD> {
		public:
			TileDatabase() {}
			virtual ~TileDatabase() {}
			
			/// @name Compatibility layer
			//@{
				const TileD &getTile(IdType type) const
				{ return *getType(type); }
				
				IdType addTile(const TileD &tile)
				{ return registerObj(new TileD(tile) ); }
			//@}
			
	};
}

#endif
