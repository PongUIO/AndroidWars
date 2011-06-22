#include "Body.h"
#include "Sync.h"

namespace Sim {
	Body::Body() :
		mPos(0),
		mVel(0),
		mAcc(0),
		
		mMass(1.0)
	{}
	
	void Body::step(double stepTime)
	{
		mVel += mAcc*stepTime;
		
		mPos += mVel*stepTime + mAcc*0.5*stepTime*stepTime;
	}
	
	void Body::checksum(Sync& sync)
	{
		sync.mixVec(mPos);
		sync.mixVec(mVel);
		sync.mixVec(mAcc);
		sync.mixFloat(mMass);
	}
}
