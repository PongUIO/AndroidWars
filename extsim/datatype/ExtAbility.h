#ifndef EXTSIM_EXTABILTIY_H
#define EXTSIM_EXTABILTIY_H

#include "../ExtBaseData.h"
#include "../TypeRule.h"

namespace ExtS {
	class ExtSim;
	
	class ExtAbility {
		public:
			ExtAbility();
			~ExtAbility();
			
			void loadBlock(Script::Block &block, TypeRule *rule);
			void postProcess(ExtSim &extsim);
			
			const std::string &getName() const { return mName; }
			const std::string &getDescription() const { return mDescription; }
			
			const TypeRule *getRule() const { return mRule; }
			Sim::IdType getId() const { return mId; }
			
		private:
			Sim::IdType mId;
			
			std::string mName;
			std::string mDescription;
			
			TypeRule *mRule;
			friend class Sim::DataCtr<ExtAbility>;
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
