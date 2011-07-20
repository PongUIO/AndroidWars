#include "StateSys.h"

namespace Sim {
	StateSys::StateSys()
	{}
	
	StateSys::~StateSys()
	{}
	
	void StateSys::finalize()
	{
		for(StateVec::iterator i=mStates.begin();
			i!=mStates.end(); i++) {
			(*i)->init(*this);
		}
	}
	
	StateSys::IdType StateSys::registerState(
		StateSys::State* state)
	{
		mStates.push_back(state);
		return mStates.size()-1;
	}

	StateSys::IdType StateSys::registerEntryPoint(
		StateSys::IdType entry)
	{
		mEntry.push_back(entry);
		return mEntry.size()-1;
	}
	
	// StateSys::State functions
	//
	//
	StateSys::IdType StateSys::State::insertChild(StateSys::State* child)
	{
		mStates.push_back(child);
		return mStates.size()-1;
	}

}
