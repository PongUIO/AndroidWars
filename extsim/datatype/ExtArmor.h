#ifndef EXTSIM_EXTARMOR_H
#define EXTSIM_EXTARMOR_H

#include <vector>
#include <string>

#include "../ExtBaseData.h"
#include "../../simulation/data/ArmorD.h"


namespace ExtS {
	class ExtArmorData : public ExtBaseData {
		public:
			ExtArmorData(ExtSim& esim);
			virtual ~ExtArmorData();
			
			void startup() {}
			void shutdown() {}
			
			void loadBlock(Script::Block& block);
			void postProcess();
			
		private:
			struct RuleData {
				Sim::ArmorD *mSrc;
				
				struct Rule : public Sim::ArmorD::DamageRule {
					std::string mDmgType;
				};
				
				typedef std::vector<Rule> RuleVec;
				RuleVec mRule;
			};
			
			RuleData::Rule loadRuleData(const Script::Data &data,
				bool loadType=true);
			
			typedef std::vector<RuleData> RuleDataVec;
			RuleDataVec mRuleData;
	};
}

#endif