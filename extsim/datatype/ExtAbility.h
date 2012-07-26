#ifndef EXTSIM_EXTABILTIY_H
#define EXTSIM_EXTABILTIY_H

#include "../DefaultExtData.h"
#include "../object/ExtDataObj.h"

namespace ExtS {
	class ExtSim;
	
	class ExtAbility : public ExtDataObjBase {
		public:
			ExtAbility(ExtSim *esim);
			~ExtAbility();
			
			void loadNode(Nepeta::Node &node);
			void postProcess(ExtSim &extsim);
			
	};
	
	class ExtAbilityData : public DefaultExtData<ExtAbility> {
		public:
			ExtAbilityData(ExtSim &esim);
			virtual ~ExtAbilityData();
			
			void startup() {}
			void shutdown() {}
	};
}

#endif
