#include "ExtArmor.h"
#include "../ExtData.h"

#include "../ExtSim.h"
#include "../../simulation/data/WeaponD.h"

namespace ExtS {
	ExtArmorData::ExtArmorData(ExtSim &esim): ExtBaseData(esim)
	{}
	
	ExtArmorData::~ExtArmorData()
	{}
	
	void ExtArmorData::loadBlock(Script::Block& block)
	{
		const std::string &name = block.getDataFirst("Name");
		if(name.empty())
			return;
		
		Sim::ArmorD *armorType =
			mExtSim->getSim().getData().getArmorDb().newArmor(name);
		
		RuleData::Rule defaultRule = loadRuleData(block.getDataSimple("Default"), false);
		armorType->setDefaultRule(defaultRule);
		
		RuleData ruleData;
		ruleData.mSrc = armorType;
		
		Script::Block::DataRange ruleRange = block.getData("Rule");
		for(Script::Block::DataMap::iterator i=ruleRange.first;
			i!=ruleRange.second; i++) {
			ruleData.mRule.push_back( loadRuleData(i->second, true) );
		}
		
		mRuleData.push_back(ruleData);
	}
	
	/**
	 * Performs a post-processing step for armor data.
	 * 
	 * This is necessary to ensure all names are properly resolved to their
	 * respective identifiers.
	 */
	void ExtArmorData::postProcess()
	{
		Sim::DamageDatabase &damageDb =
			mExtSim->getSim().getData().getDamageDb();
		
		for(RuleDataVec::iterator i=mRuleData.begin(); i!=mRuleData.end(); i++) {
			RuleData &data = *i;
			
			for(RuleData::RuleVec::iterator ri = data.mRule.begin();
				ri != data.mRule.end(); ri++) {
				RuleData::Rule &rule = *ri;
				
				Sim::IdType dId = damageDb.getIdOf(rule.mDmgType);
				if(dId != Sim::NoId) {
					data.mSrc->registerRule(dId, rule);
				}
			}
		}
		
		mRuleData.clear();
	}
	
	ExtArmorData::RuleData::Rule ExtArmorData::loadRuleData(const Script::Data& data,
		bool loadType)
	{
		int ind=0;
		RuleData::Rule rule;
		
		if(loadType)
			rule.mDmgType = data.getArg(ind++);
		rule.mMultiplier = ExtData::readValue<double>(data.getArg(ind++), 1.0);
		rule.mIsIgnoring = ExtData::readValue<bool>(data.getArg(ind++), false);
		
		return rule;
	}


}
