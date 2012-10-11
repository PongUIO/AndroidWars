#ifndef EXTS_PARAMVISITOR_H
#define EXTS_PARAMVISITOR_H

#include <stdint.h>

namespace Sim {
	class Bot;
	class Program;
}

namespace exts {
	// Forward declare classes
	template<class T>
	class IdParam;
	
	template<class T>
	class ValueParam;
	
	class PositionParam;
	
	/**
	* @brief Interface for a parameter visitor
	*/
	class ParamVisitor {
		public:
			// ValueParam visitors for all types in valtype.def
			#define _EXTS_X(type) virtual void visit(ValueParam<type> &) {}
			#include "../param/valtype.def"
			#undef _EXTS_X
			
			// IdList visitors for all types in dataid.def
			#define _EXTS_X(type) virtual void visit(IdParam<type> &) {}
			#include "../param/dataid.def"
			#undef _EXTS_X
			
			// Special visitors
			virtual void visit(ValueParam<Sim::Vector> &) {}
			virtual void visit(PositionParam &) {}
	};
}

#endif
