#ifndef SIM_BODY_H
#define SIM_BODY_H

#include "Vector.h"
#include "Save.h"

namespace Sim {
	class Body : public Save::OperatorImpl<Body> {
		public:
			Body();
			
			Vector mPos;
			Vector mVel;
			Vector mAcc;
			
			double mMass;
			
			void step(double stepTime);
			void addMomentum(const Vector &mom)
			{ mVel += mom/mMass; }
			
			void save(Save::BasePtr &fp) const
			{ fp << mPos << mVel << mAcc << mMass; }
			void load(Save::BasePtr &fp)
			{ fp >> mPos >> mVel >> mAcc >> mMass; }
			
		private:
			
	};
	
}

#endif
