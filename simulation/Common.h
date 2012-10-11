#ifndef SIM_COMMON_H
#define SIM_COMMON_H

#include <stdint.h>
#include <exception>

namespace Sim {
	/// Common type for numerical identifiers
	typedef uint32_t IdType;
	
	/// Special identifier "reserved" as an invalid identifier.
	static const IdType NoId = -1;
	
	/// Common type for size
	typedef uint32_t SizeType;
	
	/// @name Exceptions for the simulation
	//@{
		/**
		 * @brief Thrown when an illegal identifier is found.
		 * 
		 * When an illegal identifier is passed as a parameter, or detected
		 * by internal processes, this is thrown.
		 */
		class IllegalId : public std::exception {
			public:
				const char* what() const throw()
				{	return "Illegal identifier detected"; }
		};
	//@}
};

#endif
