#include "AbilityD.h"

#include "../ability/AbilityInclude.h"

namespace Sim {
	enum AbilityBaseTypes {
		TypeBase=0,
	};
	
#define _SIM_ABILITY_DEF(baseClass,		type, name)  \
	if(type == TYPE) registerCustom<Abil::baseClass>();
	
	void AbilityDatabase::registerAllDefault()
	{
#define TYPE TypeBase
#include "../ability/AbilityDef.def"
#undef TYPE
	}
}
