#include "TypeRule.h"

namespace ExtS {
	// TypeRule
	//
	//
	TypeRule::TypeRule()
	{}
	
	TypeRule::~TypeRule()
	{}
	
	bool TypeRule::checkConstrained(ParamList* srcList, ExtSim &extsim) const
	{
		for(size_t i=0; i<size(); ++i) {
			RuleParameter *referenceParam = mRuleParam[i];
			
			if(referenceParam->isConstraintUndefined())
				continue;
			
			if(!referenceParam->isValid(srcList->getParam<RuleParameter>(i),extsim))
				return false;
		}
		
		return true;
	}
	
	void TypeRule::readBlock(Script::Block* block)
	{
		for(RuleParamVec::iterator i=mRuleParam.begin(); i!=mRuleParam.end();
			++i) {
			(*i)->readBlock(block);
		}
	}
	
	void TypeRule::postProcess(ExtSim& extsim)
	{
		for(RuleParamVec::iterator i=mRuleParam.begin(); i!=mRuleParam.end();
			++i) {
			(*i)->postProcess(extsim);
		}
	}

	
	ParamList* TypeRule::makeParam() const
	{
		ParamList *pL = new ParamList();
		
		*pL = *this;
		return pL;
	}
	
	// ParamList
	//
	//
	/**
	 * Copy constructor that clones the source's parameters
	 */
	ParamList::ParamList(const ExtS::ParamList& src)
	{
		*this = src;
	}
	
	ParamList& ParamList::operator=(const ExtS::ParamList& src)
	{
		if(this != &src) {
			clearRuleParam();
			
			for(RuleParamVec::const_iterator i=src.mRuleParam.begin();
			i!=src.mRuleParam.end(); ++i) {
				registerRuleParam( (*i)->clone() );
			}
		}
		
		return *this;
	}

	
	void ParamList::clearRuleParam()
	{
		for(RuleParamVec::iterator i=mRuleParam.begin();
			i!=mRuleParam.end(); ++i)
			delete *i;
		
		mRuleParam.clear();
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
			rule->readBlock(&block);
		}
		
		return rule;
	}
}
