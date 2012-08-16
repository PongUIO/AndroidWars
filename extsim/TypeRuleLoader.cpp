#include "TypeRuleLoader.h"

#include "object/TypeRule.h"
#include "CommonLoad.h"
#include "ExtSim.h"

namespace exts {
	TypeRuleLoader::TypeRuleLoader(ExtSim& esim) : mExtSim(esim)
	{}
	
	TypeRuleLoader::~TypeRuleLoader()
	{	clear(); }
	
	void TypeRuleLoader::clear()
	{
		for(ReferenceMap::iterator i=mReferences.begin();
		i!=mReferences.end(); ++i)
			delete i->second;
		
		mReferences.clear();
	}
	
	TypeRule* TypeRuleLoader::loadRuleNode(const Nepeta::Node& node)
	{
		const std::string &id = node.getNodeFirst(Load::Base);
		
		ReferenceMap::iterator i=mReferences.find(id);
		if(i == mReferences.end())
			return 0;
		
		TypeRule *tmp = i->second->clone();
		mExtSim.getTypeRuleMgr().registerRule(tmp);
		return tmp;
	}

	void TypeRuleLoader::registerRule(const std::string& name, TypeRule* rule)
	{	mReferences[name] = rule; }
}
