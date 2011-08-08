#ifndef SIM_TILED_H
#define SIM_TILED_H

#include <vector>
#include <stdint.h>
#include "../Save.h"

#include "BaseData.h"

namespace Sim {
	struct TileD {
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
	
	class TileDatabase : public DataT<TileD,uint16_t> {
		public:
			TileDatabase() {}
			virtual ~TileDatabase() {}
			
			/// @name Compatibility layer
			//@{
				const TileD &getTile(uint16_t type) const
				{ return *getType(type); }
				
				uint16_t addTile(const TileD &tile)
				{ return addType(new TileD(tile) ); }
			//@}
			
			void save(Save::BasePtr &fp) {
				fp.writeInt<uint32_t>(mData.size());
				for(DataVec::iterator i=mData.begin(); i!=mData.end(); i++) {
					TileD *t = *i;
					
					fp.writeInt(t->colMask);
					fp.writeFloat(t->blastResist);
					fp.writeFloat(t->bounce);
					fp.writeFloat(t->friction);
				}
			}
			
	};
}

#endif
