#include "Data.h"
#include "Simulation.h"

namespace Sim {
	Data::Data(Simulation *sim) :
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
		
		callArg(&BaseData::startup, mSim);
		
		mTileCol.startup(cfg.tileSize, 8);
	}
	
	void Data::shutdown()
	{
		mTileCol.shutdown();
		
		rcall(&BaseData::shutdown);
	}
	
	void Data::save(Save::BasePtr &fp)
	{
		mTile.save(fp);
	}
}
