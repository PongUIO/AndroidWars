#ifndef SIM_BULLETD_H
#define SIM_BULLETD_H

#include <vector>
#include <stdint.h>
#include "../Save.h"

//#include "../bullet/BulletSys.h"

#include "BaseData.h"

namespace Sim {
	struct BulletD {
		
	};
	
	class BulletDatabase : public DataT<BulletD> {
		public:
			BulletDatabase() {}
			virtual ~BulletDatabase() {}
			
			void save(Save::BasePtr &fp) {
			}
			
		private:
	};
}

#endif
