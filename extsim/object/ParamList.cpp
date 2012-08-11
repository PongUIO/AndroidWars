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
	
	void ParamList::save(Sim::Save::BasePtr& fp) const
	{
		throw std::runtime_error("Undefined");
		//fp << mRuleParam;
	}

	void ParamList::load(Sim::Save::BasePtr& fp)
	{
		throw std::runtime_error("Undefined");
		//fp >> mRuleParam;
	}
	
	bool ParamList::isConstrained(ExtSim &ref) const
	{
		const TypeRule *R = ref.getTypeRuleMgr().getRule(mRefTypeRuleId);
		if(!R)
			return false;
		
		return R->checkConstrained(this);
	}
}
