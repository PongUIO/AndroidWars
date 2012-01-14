#ifndef SIM_DAMAGED_H
#define SIM_DAMAGED_H

#include "BaseData.h"

namespace Sim {
	/**
	 * Contains information about a damage type.
	 * 
	 * @note A damage type do not need to store any data at present.
	 * However, they are still important in terms of identifying damage for
	 * armor types.
	 */
	class DamageD {
		public:
			DamageD() {}
			~DamageD() {}
			
			uint32_t getId() const { return mId; }
			
		private:
			uint32_t mId;
			
			friend class DamageDatabase;
	};
	
	/**
	 * Manages all damage types.
	 */
	class DamageDatabase : public DataT<DamageD> {
		public:
			DamageDatabase() {}
			virtual ~DamageDatabase() {}
			
			const DamageD *getDamage(uint32_t id) const
			{ return getType(id); }
			
			const DamageD *getDamage(const std::string &name) const
			{ return getType(mNameIdMgr.getIdOf(name)); }
			
			uint32_t getIdOf(const std::string &name) const
			{ return mNameIdMgr.getIdOf(name); }
			
			DamageD *newDamage(const std::string &name)
			{
				DamageD *data = new DamageD();
				uint32_t id = addType(data);
				data->mId = id;
				
				mNameIdMgr.connect(id, name);
				
				return data;
			}
		
		private:
			NameIdMgr mNameIdMgr;
	};
}

#endif
