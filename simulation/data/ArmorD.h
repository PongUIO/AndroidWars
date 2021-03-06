#ifndef SIM_ARMORD_H
#define SIM_ARMORD_H

#include <boost/unordered_map.hpp>
#include "BaseData.h"

namespace Sim {
	class ArmorDatabase;
	
	class ArmorD {
		public:
			typedef ArmorDatabase DatabaseType;
			
			struct DamageRule {
				DamageRule(double multiplier=1.0, bool isIgnoring=false) :
					mMultiplier(multiplier), mIsIgnoring(isIgnoring) {}
				
				double mMultiplier;
				bool mIsIgnoring;
			};
			typedef boost::unordered_map<IdType, DamageRule> RuleMap;
			
			ArmorD() {}
			~ArmorD() {}
			
			const DamageRule &getDamageRule(IdType dmgType) const {
				RuleMap::const_iterator i=mRules.find(dmgType);
				return (i==mRules.end()) ? mDefault : i->second;
			}
			
			void setDefaultRule(const DamageRule &data) { mDefault = data; }
			void registerRule(IdType dmgType, const DamageRule &data)
			{ mRules[dmgType] = data; }
			
			const RuleMap &getRules() const { return mRules; }
			const DamageRule &getDefault() const { return mDefault; }
			
			IdType getId() const { return mId; }
			
		private:
			DamageRule mDefault;
			RuleMap mRules;
			
			IdType mId;
			
			friend class ArmorDatabase;
	};
	
	class ArmorDatabase : public DefaultDatabase<ArmorD> {
		public:
			ArmorDatabase() {}
			virtual ~ArmorDatabase() {}
			
			/// @name Compatibility layer
			//@{
				const ArmorD *getArmor(IdType id) const
				{ return getType(id); }
				
				const ArmorD *getArmor(const std::string &name) const
				{ return getType(name); }
				
				ArmorD *newArmor(const std::string &name)
				{
					IdType typeId = registerObj(0,name);
					ArmorD *data = new ArmorD();
					reseatObj(typeId, data);
					
					return data;
				}
			//@}
			
	};
}

#endif
