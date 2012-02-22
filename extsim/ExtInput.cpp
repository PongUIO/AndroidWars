#include "ExtInput.h"
#include "ExtSim.h"

namespace ExtS {
#define _EXTS_X(type, name) \
	template<> type &ExtInput::getComponent() \
	{ return get##name(); }
	_EXTS_X_EXTINPUT_COMPONENTS
#undef _EXTS_X
	
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
