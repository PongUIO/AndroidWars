#ifndef SIM_DATA_H
#define SIM_DATA_H

#include "Save.h"
#include "data/TileD.h"
#include "data/BotD.h"
#include "collision/TileCol.h"

namespace Sim {
	class Simulation;
	
	class Data {
		public:
			Data(Simulation *sim);
			~Data();
			
			void startup();
			void shutdown();
			
			TileDatabase &getTileDb()
			{ return mTile; }
			
			BotDatabase &getBotDb()
			{ return mBot; }
			
			TileCol &getTileCol()
			{ return mTileCol; }
			
			void save(Save::BasePtr &sync);
			
		private:
			TileDatabase mTile;
			BotDatabase mBot;
			TileCol mTileCol;
			
			Simulation *mSim;
	};
}

#endif
