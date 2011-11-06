#include "BulletD.h"

namespace Sim {
	BulletD::BulletD() : mSys(new StateSys()), mId(0)
	{}
	
	
	BulletD::~BulletD()
	{
		if(mSys)
			delete mSys;
	}
}
