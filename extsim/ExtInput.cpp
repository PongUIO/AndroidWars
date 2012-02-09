#include "ExtInput.h"
#include "ExtSim.h"

namespace ExtS {
	// ExtInput
	//
	//
	ExtInput::ExtInput(ExtSim& esim) : mExtSim(esim),
		mProgram(*this,&esim.getSim()),
		
		mConstraintMode(IcmNone),
		mPlayerId(Sim::NoId)
	{}
	
	ExtInput::~ExtInput()
	{}
	
}
