#include "StateSys.h"

namespace Sim {
	StateSys::StateSys()
	{}
	
	StateSys::~StateSys()
	{
		for(StateVec::iterator i = mStates.begin(); i!=mStates.end(); ++i) {
			delete *i;
		}
	}
	
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
		state->setId(mStates.size()-1);
		return state->getId();
	}

	StateSys::IdType StateSys::registerEntryPoint(
		StateSys::IdType entry)
	{
		mEntry.push_back(entry);
		return mEntry.size()-1;
	}
	
	void StateSys::Reference::save(Save::BasePtr& fp, const ArgSave &argSave)
	{
		fp.writeInt<IdType>(mThreads.size());
		for(ThreadList::iterator i=mThreads.begin(); i!=mThreads.end(); ++i) {
			Thread &t = *i;
			fp.writeInt<IdType>(t.mActive->getId());
			fp.writeFloat(t.mDelay);
			fp.writeInt<IdType>(t.mEntryType);
			argSave.save(fp, t);
		}
	}
	
	void StateSys::Reference::load(Save::BasePtr& fp, const ArgSave &argSave,
		StateSys* sys)
	{
		mSystem = sys;
		
		IdType size = fp.readInt<IdType>();
		for(IdType i=0; i<size; ++i) {
			mThreads.push_back(Thread(this));
			Thread &t = mThreads.back();
			
			t.mActive = sys->getState(fp.readInt<IdType>());
			t.mDelay = fp.readFloat();
			t.mEntryType = fp.readInt<IdType>();
			argSave.load(fp, t);
		}
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
