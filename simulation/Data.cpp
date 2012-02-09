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
		registerCallObj(&mArmor);
		registerCallObj(&mDamage);
		registerCallObj(&mTileCol);
	}
	
	Data::~Data()
	{}
	
	void Data::startup()
	{
		const Configuration &cfg = mSim->getConfig();
		
		call( boost::bind(&BaseData::startup, _1, mSim) );
	}
	
	void Data::shutdown()
	{
		rcall( boost::bind(&BaseData::shutdown, _1) );
	}
	
	void Data::save(Save::BasePtr &fp)
	{
	}
}
