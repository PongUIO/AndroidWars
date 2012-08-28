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
	
	Agent* AgentMgr::getAgent(Sim::IdType id)
	{	return (id < mAgents.size()) ? mAgents[id] : 0; }

}
