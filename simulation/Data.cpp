#include "Data.h"
#include "Simulation.h"

namespace Sim {
	Data::Data(Simulation *sim) :
		mSim(sim)
	{
		registerCallObj(&mTile);
		registerCallObj(&mBot);
		registerCallObj(&mBullet);
		registerCallObj(&mWeapon);
		registerCallObj(&mProgram);
		registerCallObj(&mAbility);
	}
	
	Data::~Data()
	{}
	
	void Data::startup()
	{
		const Configuration &cfg = mSim->getConfig();
		
		call( boost::bind(&BaseData::startup, _1, mSim) );
		
		mTileCol.startup(cfg.tileSize, 8);
	}
	
	void Data::shutdown()
	{
		mTileCol.shutdown();
		
		rcall( boost::bind(&BaseData::shutdown, _1) );
	}
	
	void Data::save(Save::BasePtr &fp)
	{
		mTile.save(fp);
	}
}
