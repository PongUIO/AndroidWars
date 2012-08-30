#include "TypeRuleMgr.h"

#include "object/TypeRule.h"

namespace exts {
	TypeRuleMgr::TypeRuleMgr(ExtSim& esim) : mExtSim(esim)
	{}
	
	TypeRuleMgr::~TypeRuleMgr()
	{}
	
	void TypeRuleMgr::startup()
	{}

	void TypeRuleMgr::shutdown()
	{
		clear();
	}
	
	Sim::IdType TypeRuleMgr::registerRule(TypeRule* rule)
	{	Sim::IdType id = add(rule);
		rule->setId(id);
		return id;
	}

	const TypeRule* TypeRuleMgr::getRule(Sim::IdType id) const
	{	return get(id); }
	
	
	void TypeRuleMgr::saveParamList(Sim::Save::BasePtr& fp,
	const ParamList *ref) const
	{
		fp << ref->getTypeRuleId();
		ref->save(fp);
	}
	
	ParamList* TypeRuleMgr::loadParamList(Sim::Save::BasePtr& fp) const
	{
		Sim::IdType id;
		fp >> id;
		
		const TypeRule *rule = getRule(id);
		if(!rule)
			return 0;
		
		ParamList *param = rule->makeParam();
		param->load(fp);
		
		return param;
	}
}
