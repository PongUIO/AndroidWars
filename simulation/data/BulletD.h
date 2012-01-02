#ifndef SIM_BULLETD_H
#define SIM_BULLETD_H

#include "BaseData.h"

namespace Sim {
	class Bullet;
	
	class BulletDatabase : public DataBehaviourT<Bullet> {
		public:
			BulletDatabase() {}
			virtual ~BulletDatabase() {}
			
		private:
	};
}

#endif
