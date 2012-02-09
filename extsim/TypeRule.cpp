#include "TypeRule.h"

namespace ExtS {
	// TypeRule
	//
	//
	TypeRule::TypeRule()
	{}
	
	TypeRule::~TypeRule()
	{
		for(MetaParamVec::iterator i=mMetaParam.begin();
			i!=mMetaParam.end(); ++i)
			delete *i;
	}
	
	TypeRule* TypeRule::clone()
	{
		TypeRule *copy = new TypeRule();
		
		copy->mMetaParam.reserve(mMetaParam.size());
		for(MetaParamVec::iterator i=mMetaParam.begin();
			i!=mMetaParam.end(); ++i) {
			copy->mMetaParam.push_back( (*i)->clone() );
		}
		
		return copy;
	}

	
	ParamList* TypeRule::makeParam() const
	{
		ParamList *pL = new ParamList();
		
		for(MetaParamVec::const_iterator i=mMetaParam.begin(); i!=mMetaParam.end();
			++i) {
			pL->insertParam( (*i)->constructParam() );
		}
		
		return pL;
	}
	
	void TypeRule::readConstraint(Script::Block& constraintBlock)
	{
		for(MetaParamVec::iterator i=mMetaParam.begin(); i!=mMetaParam.end();
			++i) {
			MetaParam *meta = *i;
			
			meta->readConstraint(
				constraintBlock.getDataSimple(meta->getDataName()));
		}
	}
	
	void TypeRule::readParam(Script::Block& paramBlock)
	{
		for(MetaParamVec::iterator i=mMetaParam.begin(); i!=mMetaParam.end();
			++i) {
			MetaParam *meta = *i;
			meta->readParam(
				paramBlock.getDataSimple(meta->getDataName()));
		}
	}
	
	void TypeRule::postProcess(ExtSim& extsim)
	{
		for(MetaParamVec::iterator i=mMetaParam.begin(); i!=mMetaParam.end();
			++i)
			(*i)->postProcess(extsim);
	}

	
	
	// TypeRuleMgr
	//
	//
	TypeRuleMgr::TypeRuleMgr()
	{}
	
	TypeRuleMgr::~TypeRuleMgr()
	{
		for(RuleMap::iterator i=mRuleMap.begin(); i!=mRuleMap.end(); ++i)
			delete i->second;
	}
	
	/**
	 * Reads a script block, creating a TypeRule object with any script-defined
	 * parameters and constraints.
	 * 
	 * @return A newly created TypeRule object if the script block matched
	 * a rule, or NULL if no rule matched.
	 */
	TypeRule *TypeRuleMgr::loadRuleBlock(Script::Block& block)
	{
		const std::string &baseRuleName = block.getDataFirst("Base");
		TypeRule *rule = 0;
		
		RuleMap::iterator i = mRuleMap.find(baseRuleName);
		if(i != mRuleMap.end()) {
			rule = i->second->clone();
			
			Script::Block *paramBlock = block.getBlock("PARAM");
			Script::Block *constraintBlock = block.getBlock("CONSTRAINT");
			
			// Weird construct because I can
			paramBlock ? rule->readParam(*paramBlock) : (void)0;
			constraintBlock ? rule->readConstraint(*constraintBlock) : (void)0;
		}
		
		return rule;
	}
}
