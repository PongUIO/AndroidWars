#include "Body.h"

namespace Sim {
	Body::Body() :
		mPos(0),
		mVel(0),
		mAcc(0),
		mMom(0),
		
		mMass(1.0)
	{}
	
	void Body::step(double stepTime)
	{
		mVel += mMom/mMass;
		mMom = 0;
		
		mVel += mAcc*stepTime;
		
		mPos += mVel*stepTime + mAcc*0.5*stepTime*stepTime;
	}
}
