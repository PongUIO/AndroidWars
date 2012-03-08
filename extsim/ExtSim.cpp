#include "ExtSim.h"

namespace ExtS {
#define _EXTS_X(type, name) \
	template<> type &ExtSim::getComponent() \
	{ return get##name(); }
	_EXTS_X_EXTSIM_COMPONENTS
#undef _EXTS_X
	
	ExtSim::ExtSim() :
		mSim(),
		
		mData(*this),
		mInput(*this)
	{}

	ExtSim::~ExtSim()
	{}
	
	void ExtSim::startup()
	{
		mSim.startup();
	}

	void ExtSim::shutdown()
	{
		mSim.shutdown();
	}
	
	void ExtSim::prepareSim()
	{
		mSim.prepareSim();
	}
}
