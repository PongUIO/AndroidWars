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
			
			IdType getId() const { return mId; }
			
		private:
			IdType mId;
			
			friend class DamageDatabase;
	};
	
	/**
	 * Manages all damage types.
	 */
	class DamageDatabase : public DataT<DamageD> {
		public:
			DamageDatabase() {}
			virtual ~DamageDatabase() {}
			
			const DamageD *getDamage(IdType id) const
			{ return getDataById(id); }
			
			const DamageD *getDamage(const std::string &name) const
			{ return getDataById(mNameIdMgr.getIdOf(name)); }
			
			IdType getIdOf(const std::string &name) const
			{ return mNameIdMgr.getIdOf(name); }
			
			std::string getNameOf(IdType id) const
			{ return mNameIdMgr.getNameOf(id); }
			
			DamageD *newDamage(const std::string &name)
			{
				DamageD *data = new DamageD();
				IdType id = addType(data);
				data->mId = id;
				
				mNameIdMgr.connect(id, name);
				
				return data;
			}
		
		private:
			NameIdMgr mNameIdMgr;
	};
}

#endif
