#include "ExtSim.h"

namespace exts {
#define _EXTS_X(type, name) \
	template<> type &ExtSim::getComponent() \
	{ return get##name(); }
	_EXTS_X_EXTSIM_COMPONENTS
#undef _EXTS_X
	
	ExtSim::ExtSim() :
		mSim(),
		
		mData(*this),
		mInput(*this),
		mTypeRuleMgr(*this),
		mReplay(*this),
		mCpuInput(*this)
	{}

	ExtSim::~ExtSim()
	{}
	
	void ExtSim::startup()
	{
		mSim.startup();
		
		mCpuInput.startup();
	}
	
	void ExtSim::shutdown()
	{
		mCpuInput.shutdown();
		
		mSim.shutdown();
	}
	
	void ExtSim::prepareSim()
	{
		mSim.prepareSim();
		
		// Initial commit
		getReplay().commit();
	}
	
	/**
	 * Resets the extended simulation to a state as if it
	 * was just constructed. Effectively clearing out any changes
	 * since it was first created.
	 */
	void ExtSim::reset()
	{
		this->~ExtSim();
		new(this) ExtSim();
	}
}
