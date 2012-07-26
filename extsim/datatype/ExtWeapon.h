#ifndef EXTSIM_WEAPON_H
#define EXTSIM_WEAPON_H

#include "../DefaultExtData.h"
#include "../object/ExtDataObj.h"

namespace ExtS {
	class ExtWeapon : public ExtDataObjBase {
		public:
			ExtWeapon(ExtSim &esim, Sim::IdType id);
			~ExtWeapon();
			
			void loadNode(const Nepeta::Node &node);
			void postProcess();
			
		private:
			std::string mSizeType;
			std::string mBaseDamageStr;
			
			Sim::IdType mBaseDamageType;
	};
	
	class ExtWeaponData : public DefaultExtData<ExtWeapon> {
		public:
			ExtWeaponData(ExtSim& esim);
			virtual ~ExtWeaponData();
			
			Nepeta::Node &getTypeRuleNode(Nepeta::Node& node)
			{ return node.getNode("CREATERULE"); }
			
			void startup() {}
			void shutdown() {}
	};
}

#endif
