#ifndef EXTSIM_EXTABILTIY_H
#define EXTSIM_EXTABILTIY_H

#include "../ExtBaseData.h"
#include "../TypeRule.h"

namespace ExtS {
	class ExtSim;
	
	class ExtAbility : public ExtBaseDataObj {
		public:
			ExtAbility(ExtSim *esim);
			~ExtAbility();
			
			void loadBlock(Script::Block &block,
				Sim::IdType simTypeId, TypeRule *rule);
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
