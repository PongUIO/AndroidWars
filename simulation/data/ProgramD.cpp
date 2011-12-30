#include "ProgramD.h"

#include "../program/ProgramInclude.h"

namespace Sim {
	const uint32_t ProgramDatabase::NoId = -1;
	
	/// @note This constructs all base program class types using magic
#define _SIM_PROGRAM_DEF(baseClass,		type,	name) \
	const std::string &Prog::baseClass::DataSrc::getTypeName() { \
		static std::string typeName = name; \
		return typeName; } \
	const std::string &Prog::baseClass::DataSrc::getTypeNameVirt() const { \
		return getTypeName(); } \
	\
	Program *Prog::baseClass::DataSrc::createProgram(Simulation *sim, \
		uint32_t id) const \
	{	return new Prog::baseClass(sim, id, mId, Prog::baseClass::Config()); }
	
	#include "../program/ProgramDef.def"
#undef _SIM_PROGRAM_DEF
	
	/**
	 * Used to retrieve the dynamic ID for a given program type.
	 * This is here to aid the creation of programs.
	 * 
	 * @return The numerical ID of a specific program type,
	 * or \c ProgramDatabase::NoId if the program is not implemented.
	 */
	uint32_t ProgramDatabase::getTypeIdOf(const std::string& type) const
	{
		TypeMap::const_iterator i= mTypeMap.find(type);
		return (i==mTypeMap.end()) ? NoId : i->second;
	}
	
	uint32_t ProgramDatabase::registerCustom(ProgramD* progType)
	{
		uint32_t id = addType(progType);
		progType->mId = id;
		
		mTypeMap[progType->getTypeNameVirt()] = id;
		
		return id;
	}
	
	enum ProgramBaseTypes {
		TypeBase=0,
		TypeAbility
	};

#define _SIM_PROGRAM_DEF(baseClass,		type,	name) \
	if(type == TYPE) registerCustom(new Prog::baseClass::DataSrc());
	
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
