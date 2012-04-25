#include "Data.h"
#include "Simulation.h"

namespace Sim {
#define _SIM_X(type, name) \
	template<> type &Data::getDatabase() \
	{ return get##name##Db(); }
	_SIM_X_DATA_COMPONENTS
#undef _SIM_X
	
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
		call( boost::bind(&BaseDatabase::startup, _1, mSim) );
	}
	
	void Data::shutdown()
	{
		rcall( boost::bind(&BaseDatabase::shutdown, _1) );
	}
	
	void Data::save(Save::BasePtr &fp)
	{
	}
}
