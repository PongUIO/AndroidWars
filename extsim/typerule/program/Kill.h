#ifndef EXTSIM_KILL_H
#define EXTSIM_KILL_H

#include "../../object/TypeRule.h"

namespace exts { namespace prog {
	class Kill : public TypeRule {
		public:
			Kill(ExtSim& esim);
			~Kill();
			
			TypeRule* clone() { return new Kill(*this); }
			
			void makeInput(const ParamList* param) const;
			void buildTimelineData(const ParamList* param) const {}
			Sim::IdType registerSimData(const std::string& name) const;
			
		private:
	};
} }

#endif
