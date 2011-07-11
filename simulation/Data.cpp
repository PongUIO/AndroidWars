#include "Data.h"
#include "Simulation.h"

namespace Sim {
	Data::Data(Simulation *sim) : mSim(sim)
	{}
	
	Data::~Data()
	{}
	
	void Data::startup()
	{
		const Configuration &cfg = mSim->getConfig();
		
		mTile.startup();
		mTileCol.startup(cfg.tileSize, 8);
	}
	
	void Data::shutdown()
	{
		mTileCol.shutdown();
		mTile.shutdown();
	}
	
	void Data::checksum(Save::SyncPtr &sync)
	{
		mTile.checksum(sync);
	}
}
