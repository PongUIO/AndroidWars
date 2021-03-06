#include "ExtArmor.h"

#include "../ExtData.h"
#include "../util/TypeConv.h"

#include "../ExtSim.h"
#include "../../simulation/data/WeaponD.h"

namespace exts {
	ExtArmorData::ExtArmorData(ExtSim &esim): ExtDataComponent(), mExtSim(esim)
	{}
	
	ExtArmorData::~ExtArmorData()
	{}
	
	void ExtArmorData::loadNode(const Nepeta::Node& node)
	{
		const std::string &name = node.getNodeFirst("Name");
		if(name.empty())
			return;
		
		Sim::ArmorD *armorType =
			mExtSim.getSim().getData().getArmorDb().newArmor(name);
		
		RuleData::Rule defaultRule = loadRuleData(node.getNode("Default"), false);
		armorType->setDefaultRule(defaultRule);
		
		RuleData ruleData;
		ruleData.mSrc = armorType;
		
		for(size_t i=node.indexOf("Rule"); i!=Nepeta::NoPos;
		i=node.nextIndexOf("Rule",i)) {
			ruleData.mRule.push_back( loadRuleData(node.getNode(i), true) );
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
			mExtSim.getSim().getData().getDamageDb();
		
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
	
	ExtArmorData::RuleData::Rule ExtArmorData::loadRuleData(
		const Nepeta::Node& node, bool loadType)
	{
		int ind=0;
		RuleData::Rule rule;
		
		if(loadType)
			rule.mDmgType = node.getArg(ind++);
		rule.mMultiplier = convValue<double>(node.getArg(ind++), 1.0);
		rule.mIsIgnoring = convValue<bool>(node.getArg(ind++), false);
		
		return rule;
	}


}
