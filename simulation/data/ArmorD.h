#ifndef SIM_ARMORD_H
#define SIM_ARMORD_H

#include <boost/unordered_map.hpp>
#include "BaseData.h"

namespace Sim {
	class ArmorD {
		public:
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
			
			IdType getId() const { return mId; }
			
		private:
			DamageRule mDefault;
			RuleMap mRules;
			
			IdType mId;
			
			friend class ArmorDatabase;
	};
	
	class ArmorDatabase : public DataT<ArmorD> {
		public:
			ArmorDatabase() {}
			virtual ~ArmorDatabase() {}
			
			const ArmorD *getArmor(IdType id) const
			{ return getType(id); }
			
			const ArmorD *getArmor(const std::string &name) const
			{ return getType(mNameIdMgr.getIdOf(name)); }
			
			IdType getIdOf(const std::string &name) const
			{ return mNameIdMgr.getIdOf(name); }
			
			std::string getNameOf(IdType id) const
			{ return mNameIdMgr.getNameOf(id); }
			
			ArmorD *newArmor(const std::string &name)
			{
				ArmorD *data = new ArmorD();
				IdType id = addType(data);
				data->mId = id;
				
				mNameIdMgr.connect(id,name);
				
				return data;
			}
			
		private:
			NameIdMgr mNameIdMgr;
	};
}

#endif
