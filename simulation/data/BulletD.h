#ifndef SIM_BULLETD_H
#define SIM_BULLETD_H

#include <vector>
#include <stdint.h>
#include "../Save.h"

#include "../statesys/StateSys.h"

#include "BaseData.h"

namespace Sim {
	class BulletDatabase;
	
	class BulletD {
		public:
			BulletD();
			~BulletD();
			
			StateSys *getStateSys() { return mSys; }
			const StateSys *getStateSys() const { return mSys; }
			uint32_t getId() { return mId; }
			
		private:
			StateSys *mSys;
			uint32_t mId;
			
			friend class BulletDatabase;
	};
	
	class BulletDatabase : public DataT<BulletD*> {
		public:
			BulletDatabase() {}
			virtual ~BulletDatabase();
			
			BulletD *createType() {
				BulletD *data = new BulletD();
				uint32_t id = addType(data);
				data->mId = id;
				return data;
			}
			
			void save(Save::BasePtr &fp) {
				(void)fp;
			}
			
		private:
	};
}

#endif
