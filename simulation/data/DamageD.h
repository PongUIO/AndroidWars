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
	class DamageDatabase : public DefaultDatabase<DamageD> {
		public:
			DamageDatabase() {}
			virtual ~DamageDatabase() {}
			
			/// @name Compatibility layer
			//@{
				const DamageD *getDamage(IdType id) const
				{ return getType(id); }
				
				const DamageD *getDamage(const std::string &name) const
				{ return getType(name); }
				
				DamageD *newDamage(const std::string &name)
				{
					IdType typeId = registerObj(0,name);
					DamageD *data = new DamageD();
					reseatObj(typeId,data);
					
					return data;
				}
			//@}
	};
}

#endif
