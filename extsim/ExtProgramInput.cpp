#include "ExtInput.h"
#include "ExtSim.h"

#include "datatype/Program.h"

namespace ExtS {
	Sim::Program* ExtProgramInput::buildObject(Sim::Save::BasePtr &fp)
	{
		return mParent.getExtSim().getSim().getState().getProgramFactory().
			createSerialized(fp, false);
	}

	bool ExtProgramInput::isObjectConstrained(Sim::Program* prog)
	{
		return false;
	}
}
