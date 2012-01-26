#include "Armor.h"

#include "../../simulation/Simulation.h"
#include "../../simulation/data/WeaponD.h"

namespace ExtS {
	ArmorData::ArmorData(Sim::Simulation& sim): BaseData(sim)
	{}
	
	ArmorData::~ArmorData()
	{}
	
	void ArmorData::loadBlock(Script::Block& block)
	{
		const std::string &name = block.getDataFirst("Name");
		if(name.empty())
			return;
		
		Sim::ArmorD *armorType = mSim.getData().getArmorDb().newArmor(name);
		
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
	void ArmorData::postProcess()
	{
		Sim::DamageDatabase &damageDb = mSim.getData().getDamageDb();
		
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
	
	ArmorData::RuleData::Rule ArmorData::loadRuleData(const Script::Data& data,
		bool loadType)
	{
		int ind=0;
		RuleData::Rule rule;
		
		if(loadType)
			rule.mDmgType = data.getArg(ind++);
		rule.mMultiplier = readValue<double>(data.getArg(ind++), 1.0);
		rule.mIsIgnoring = readValue<bool>(data.getArg(ind++), false);
		
		return rule;
	}


}
