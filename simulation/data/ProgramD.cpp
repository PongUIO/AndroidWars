#include "ProgramD.h"

#include "../program/ProgramInclude.h"

namespace Sim {
	enum ProgramBaseTypes {
		TypeBase=0,
		TypeAbility
	};

#define _SIM_PROGRAM_DEF(baseClass,		type,	name) \
	if(type == TYPE) registerType<Prog::baseClass>();
	
	/**
	 * Registers all of the simulation's base program types.
	 * 
	 * The base program types are programs which manage basic behaviour,
	 * excluding all abilities.
	 */
	void ProgramDatabase::registerBase()
	{
#define TYPE TypeBase
#include "../program/ProgramDef.def"
#undef TYPE
	}
	
	/**
	 * Registers all of the simulation's ability program types.
	 */
	void ProgramDatabase::registerAbility()
	{
#define TYPE TypeAbility
#include "../program/ProgramDef.def"
#undef TYPE
	}
	
#undef _SIM_PROGRAM_DEF
	
	/**
	 * Registers all of the simulation's inbuild program types.
	 */
	void ProgramDatabase::registerAllDefault()
	{
		registerBase();
		registerAbility();
	}
}
