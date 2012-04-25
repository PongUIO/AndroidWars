#ifndef EXTSIM_WEAPON_H
#define EXTSIM_WEAPON_H

#include "../ExtBaseData.h"

namespace ExtS {
	class ExtWeapon : public ExtBaseDataObj {
		public:
			ExtWeapon(ExtSim *esim);
			~ExtWeapon();
			
			void loadBlock(Script::Block &block,
				Sim::IdType simTypeId, TypeRule *rule);
			void postProcess(ExtSim &extsim);
			
		private:
			std::string mSizeType;
			std::string mBaseDamageStr;
			
			Sim::IdType mBaseDamageType;
	};
	
	class ExtWeaponData : public DefaultExtData<ExtWeapon> {
		public:
			ExtWeaponData(ExtSim& esim);
			virtual ~ExtWeaponData();
			
			Script::Block &getTypeRuleBlock(Script::Block& block)
			{ return *block.getBlockSimple("CREATERULE"); }
			
			void startup() {}
			void shutdown() {}
	};
}

#endif
