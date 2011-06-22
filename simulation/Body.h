#ifndef SIM_BODY_H
#define SIM_BODY_H

#include "Vector.h"

namespace Sim {
	class Sync;
	
	class Body {
		public:
			Body();
			
			Vector mPos;
			Vector mVel;
			Vector mAcc;
			
			double mMass;
			
			void step(double stepTime);
			void addMomentum(const Vector &mom)
			{ mVel += mom/mMass; }
			
			void checksum(Sync &sync);
		private:
			
	};
	
}

#endif
