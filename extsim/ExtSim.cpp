#include "ExtSim.h"

namespace ExtS {
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
