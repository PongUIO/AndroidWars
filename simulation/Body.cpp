#include "Body.h"

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
	
	void Body::save(Save::BasePtr &fp)
	{
		fp.writeVec(mPos);
		fp.writeVec(mVel);
		fp.writeVec(mAcc);
		fp.writeFloat(mMass);
	}
	
	void Body::load(Save::BasePtr& fp)
	{
		mPos = fp.readVec();
		mVel = fp.readVec();
		mAcc = fp.readVec();
		mMass = fp.readFloat();
	}
}
