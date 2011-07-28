#ifndef SIM_WEAPOND_H
#define SIM_WEAPOND_H

#include "../statesys/StateSys.h"

#include "BaseData.h"

namespace Sim {
	class WeaponD {
		public:
			WeaponD();
			~WeaponD();
			
			StateSys *getStateSys() { return mSys; }
			const StateSys *getStateSys() const { return mSys; }
			uint32_t getId() { return mId; }
			
		private:
			StateSys *mSys;
			uint32_t mId;
			
			friend class WeaponDatabase;
	};
	
	class WeaponDatabase : public DataT<WeaponD*> {
		public:
			WeaponDatabase() {}
			virtual ~WeaponDatabase();
			
			WeaponD *createType() {
				WeaponD *data = new WeaponD();
				uint32_t id = addType(data);
				data->mId = id;
				return data;
			}
			
		private:
			
	};
}

#endif