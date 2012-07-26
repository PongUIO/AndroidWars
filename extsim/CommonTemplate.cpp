#include "CommonTemplate.h"

#include "ExtSim.h"

namespace ExtS {
#define _EXTS_X(type, name) \
	template<> type &getExtSimComponent(ExtSim& extsim) \
	{ return extsim.getComponent<type>(); }
	_EXTS_X_EXTSIM_COMPONENTS
#undef _EXTS_X
	
#define _EXTS_X(type, name) \
	template<> type &getExtDataComponent(ExtSim& extsim) \
	{ return getExtSimComponent<ExtData>(extsim).getComponent<type>(); }
	_EXTS_X_EXTDATA_COMPONENTS
#undef _EXTS_X

/* #define _EXTS_X(type, name) \
	template<> type &getExtDataComponent(ExtSim& extsim) \
	{ return getExtSimComponent<ExtInput>(extsim).getComponent<type>(); }
	_EXTS_X_EXTINPUT_COMPONENTS
#undef _EXTS_X */
}
