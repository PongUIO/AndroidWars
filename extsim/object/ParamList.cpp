#include "ParamList.h"

#include "../ExtSim.h"
#include "TypeRule.h"

namespace exts {
	ParamList::ParamList(const ParamList& other)
	{
		*this = other;
	}
	
	ParamList& ParamList::operator=(const ParamList& src)
	{
		if(this != &src) {
			clearRuleParam();
			
			mRefTypeRuleId = src.mRefTypeRuleId;
			for(RuleParamVec::const_iterator i=src.mRuleParam.begin();
			i!=src.mRuleParam.end(); ++i) {
				mRuleParam.push_back( (*i)->clone() );
			}
		}
		
		return *this;
	}
	
	void ParamList::clearRuleParam()
	{
		for(RuleParamVec::iterator i=mRuleParam.begin();
		i!=mRuleParam.end(); ++i) {
			delete *i;
		}
		mRuleParam.clear();
	}
	
	/**
	 * @brief Saves a parameter list
	 * 
	 * @warning The \c ParamList is associated with a specific \c TypeRule
	 * object, therefore the list is stored without metadata.
	 */
	void ParamList::save(Sim::Save::BasePtr& fp) const
	{
		fp << mRefAgent;
		
		for(RuleParamVec::const_iterator i=mRuleParam.begin();
		i!=mRuleParam.end(); ++i) {
			fp << *(*i);
		}
	}
	
	/**
	 * @brief Loads a parameter list
	 * 
	 * @warning The \c ParamList is associated to a specific \c TypeRule object.
	 * The \c ParamList assumes it is already pre-associated and all its
	 * variables are constructed.
	 */
	void ParamList::load(Sim::Save::BasePtr& fp)
	{
		fp >> mRefAgent;
		
		for(RuleParamVec::iterator i=mRuleParam.begin();
		i!=mRuleParam.end(); ++i) {
			fp >> *(*i);
		}
	}
	
	bool ParamList::isConstrained(ExtSim &ref) const
	{
		const TypeRule *R = ref.getTypeRuleMgr().getRule(mRefTypeRuleId);
		if(!R)
			return false;
		
		return R->checkConstrained(this);
	}
	
	void ParamList::traverseVisitor(ParamVisitor& visitor)
	{
		for(RuleParamVec::iterator i=mRuleParam.begin();
		i!=mRuleParam.end(); ++i) {
			(*i)->accept(visitor);
		}
	}
}
