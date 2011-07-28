#include <stdio.h>
#include "StateSys.h"

namespace Sim {
	StateSys::Reference::Reference(const StateSys* sys) :
		mSystem(sys)
	{}
	
	StateSys::Reference::~Reference()
	{}
	
	void StateSys::Reference::startThread(
		IdType entryId, const boost::any &arg)
	{
		mThreads.push_back(
			Thread(this, mSystem->getState(entryId), entryId, arg)
		);
	}
	
	void StateSys::Reference::exec(double delta)
	{
		for(ThreadList::iterator i = mThreads.begin();
			i!=mThreads.end(); ++i) {
			Thread &t = *i;
			
			// If the thread can process, do so.
			// The thread stops when either:
			// - A new delay occurs
			// - The thread is dead
			while(true) {
				if(t.mActive && t.mDelay <= 0.0)
					t.mActive->exec(t);
				
				if(t.mDelay > 0.0) {
					t.mDelay -= delta;
					delta = 0.0;
				}
				
				if(t.mDelay > 0.0 || t.mActive==NULL)
					break;
			}
			
			// Kill the thread if it is dead
			if(t.mActive == NULL)
				i = mThreads.erase(i);
		}
	}
	
	
	// Thread functions
	//
	//
	StateSys::Reference::Thread::Thread(
		Reference *ref, const State *state, IdType type, const boost::any &arg
		) : mHost(ref), mActive(state), mDelay(0.0), mEntryType(type),
			mArg(arg)
	{}
}
