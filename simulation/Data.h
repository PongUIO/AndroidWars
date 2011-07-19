#ifndef SIM_DATA_H
#define SIM_DATA_H

#include "Save.h"
#include "data/TileD.h"
#include "data/BotD.h"
#include "data/BulletD.h"
#include "collision/TileCol.h"

#include "data/BaseData.h"
#include "utility/CallGroup.h"

namespace Sim {
	class Simulation;
	
	class Data : private CallGroup<Data,BaseData>  {
		public:
			Data(Simulation *sim);
			~Data();
			
			void startup();
			void shutdown();
			
			void save(Save::BasePtr &sync);
			
			/// @name Database retrieval
			//@{
				// Databases
				TileDatabase &getTileDb() { return mTile; }
				BotDatabase &getBotDb() { return mBot; }
				BulletDatabase &getBulletDb() { return mBullet; }
				
				// Other constant data
				TileCol &getTileCol() { return mTileCol; }
			//@}
			
		private:
			/// @name Group calls
			//@{
				void gStartup(BaseData *d) { d->startup(mSim); }
				void gShutdown(BaseData *d) { d->shutdown(); }
			//@}
			
			TileDatabase mTile;
			BotDatabase mBot;
			BulletDatabase mBullet;
			
			TileCol mTileCol;
			
			Simulation *mSim;
	};
}

#endif
