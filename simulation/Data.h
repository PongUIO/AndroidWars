#ifndef SIM_DATA_H
#define SIM_DATA_H

#include "Save.h"
#include "data/TileD.h"

namespace Sim {
	
	class Data {
		public:
			Data();
			~Data();
			
			void startup();
			void shutdown();
			
			TileDatabase &getTileDb()
			{ return mTile; }
			
			void checksum(Save::SyncPtr &sync);
			
		private:
			TileDatabase mTile;
	};
}

#endif
