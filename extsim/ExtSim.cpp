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
	{
		registerCallObj(&mData);
		registerCallObj(&mInput);
		registerCallObj(&mTypeRuleMgr);
		registerCallObj(&mReplay);
		registerCallObj(&mAgent);
		registerCallObj(&mCpuInput);
	}
	
	ExtSim::~ExtSim()
	{}
	
	void ExtSim::startup()
	{
		mSim.startup();
		
		call( boost::bind(&ExtModule::startup, _1) );
	}
	
	void ExtSim::shutdown()
	{
		rcall( boost::bind(&ExtModule::shutdown, _1) );
		
		mSim.shutdown();
	}
	
	void ExtSim::save(Sim::Save& dst)
	{
		Sim::Save::FilePtr fp(dst);
		
		getSim().save(fp);
		call( boost::bind(&ExtModule::save, _1, boost::ref(fp)) );
	}

	void ExtSim::load(const Sim::Save& saveData)
	{
		Sim::Save::FilePtr fp(saveData);
		
		getSim().load(fp);
		call( boost::bind(&ExtModule::load, _1, boost::ref(fp)) );
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
