#include "WeaponD.h"

namespace Sim {
	WeaponD::WeaponD() : mSys(new StateSys()), mId(0)
	{}
	
	WeaponD::~WeaponD()
	{
		if(mSys)
			delete mSys;
	}
	
	WeaponDatabase::~WeaponDatabase()
	{	clearData(); }

}
