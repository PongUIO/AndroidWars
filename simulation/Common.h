#ifndef SIM_COMMON_H
#define SIM_COMMON_H

#include <stdint.h>

namespace Sim {
	/// Common type for numerical identifiers
	typedef uint32_t IdType;
	
	/// Special identifier "reserved" as an invalid identifier.
	static const IdType NoId = -1;
	
	/// Common type for size
	typedef uint32_t SizeType;
};

#endif
