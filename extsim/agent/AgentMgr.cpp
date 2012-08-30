#include <limits>

#include "AgentMgr.h"

namespace exts {
	AgentMgr::AgentMgr()
	{}
	
	AgentMgr::~AgentMgr()
	{}
	
	void AgentMgr::setupAgents(Sim::IdType count)
	{
		mAgents.resize(count);
		
		Sim::IdType idCount = std::numeric_limits<Sim::IdType>::max()/count;
		for(Sim::IdType i=0; i<count; ++i) {
			mAgents[i] = new Agent(i, i*idCount, (i+1)*idCount);
		}
	}
	
	void AgentMgr::clearAgents()
	{
		for(AgentVec::iterator i=mAgents.begin(); i!=mAgents.end(); ++i)
			delete *i;
		mAgents.clear();
	}
	
	void AgentMgr::discardAllocAll()
	{
		for(AgentVec::iterator i=mAgents.begin(); i!=mAgents.end(); ++i)
			(*i)->discardAlloc();
	}
	
	void AgentMgr::updateAllocAll()
	{
		for(AgentVec::iterator i=mAgents.begin(); i!=mAgents.end(); ++i)
			(*i)->updateAlloc();
	}
	
	void AgentMgr::save(Sim::Save::BasePtr& fp) const
	{
		for(AgentVec::const_iterator i=mAgents.begin(); i!=mAgents.end(); ++i)
			fp << **i;
	}

	void AgentMgr::load(Sim::Save::BasePtr& fp)
	{
		for(AgentVec::iterator i=mAgents.begin(); i!=mAgents.end(); ++i)
			fp >> **i;
	}

	
	Agent* AgentMgr::getAgent(Sim::IdType id)
	{	return (id < mAgents.size()) ? mAgents[id] : 0; }

}
