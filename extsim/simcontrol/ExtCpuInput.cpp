#include "ExtCpuInput.h"

#include "../typerule/misc/CpuInput.h"
#include "../ExtSim.h"

namespace exts {
	ExtCpuInput::ExtCpuInput(ExtSim& esim) : mExtSim(esim)
	{}

	ExtCpuInput::~ExtCpuInput()
	{}
	
	void ExtCpuInput::startup()
	{
		mCpuInput = new misc::CpuInput(mExtSim);
		mExtSim.getTypeRuleMgr().registerRule(mCpuInput);
	}

	void ExtCpuInput::shutdown()
	{}
	
	void ExtCpuInput::registerCpuInput(Sim::IdType bot,
		Sim::IdType prog, uint32_t delay)
	{
		mExtSim.getInput().registerInput(mCpuInput->setupParam(bot,prog,delay));
	}
}
