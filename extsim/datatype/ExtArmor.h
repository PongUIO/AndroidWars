#ifndef EXTSIM_EXTARMOR_H
#define EXTSIM_EXTARMOR_H

#include <vector>
#include <string>

#include "../ExtDataComponent.h"

#include "../../simulation/data/ArmorD.h"


namespace ExtS {
	class ExtSim;
	
	class ExtArmorData : public ExtDataComponent {
		public:
			ExtArmorData(ExtSim& esim);
			virtual ~ExtArmorData();
			
			void startup() {}
			void shutdown() {}
			
			void loadNode(const Nepeta::Node& block);
			void postProcess();
			
			Sim::IdType size();
			
		private:
			ExtSim &mExtSim;
			
			struct RuleData {
				Sim::ArmorD *mSrc;
				
				struct Rule : public Sim::ArmorD::DamageRule {
					std::string mDmgType;
				};
				
				typedef std::vector<Rule> RuleVec;
				RuleVec mRule;
			};
			
			RuleData::Rule loadRuleData(const Nepeta::Node &node,
				bool loadType=true);
			
			typedef std::vector<RuleData> RuleDataVec;
			RuleDataVec mRuleData;
	};
}

#endif
