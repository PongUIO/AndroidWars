#ifndef SIM_DATA_H
#define SIM_DATA_H

#include "data/TileD.h"

namespace Sim {
	class Sync;
	
	class Data {
		public:
			Data();
			~Data();
			
			void startup();
			void shutdown();
			
			TileDatabase &getTileDb()
			{ return mTile; }
			
			void checksum(Sync &sync);
			
		private:
			TileDatabase mTile;
	};
}

#endif
