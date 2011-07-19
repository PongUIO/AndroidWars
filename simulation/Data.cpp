#include "Data.h"
#include "Simulation.h"

namespace Sim {
	Data::Data(Simulation *sim) :
		CallGroup<Data,BaseData>(this),
		mSim(sim)
	{
		registerCallObj(&mTile);
		registerCallObj(&mBot);
		registerCallObj(&mBullet);
	}
	
	Data::~Data()
	{}
	
	void Data::startup()
	{
		const Configuration &cfg = mSim->getConfig();
		
		call(&Data::gStartup);
		
		mTileCol.startup(cfg.tileSize, 8);
	}
	
	void Data::shutdown()
	{
		mTileCol.shutdown();
		
		rcall(&Data::gShutdown);
	}
	
	void Data::save(Save::BasePtr &fp)
	{
		mTile.save(fp);
	}
}
