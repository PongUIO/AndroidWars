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
		rule->mId = id;
		return id;
	}

	const TypeRule* TypeRuleMgr::getRule(Sim::IdType id) const
	{	return get(id); }

}
