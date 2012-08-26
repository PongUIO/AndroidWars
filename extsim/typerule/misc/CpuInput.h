#ifndef EXTSIM_TYPERULE_CPUINPUT_H
#define EXTSIM_TYPERULE_CPUINPUT_H

#include "../../object/TypeRule.h"

namespace exts { namespace misc {
	class CpuInput : public TypeRule {
		public:
			CpuInput(ExtSim& esim);
			~CpuInput();
			
			TypeRule* clone() { return new CpuInput(*this); }
			
			void makeInput(const ParamList* param) const;
			void buildTimelineData(const ParamList* param) const;
			Sim::IdType registerSimData(const std::string& name) const;
			
			ParamList *setupParam(Sim::IdType bot, Sim::IdType prog,
			uint32_t delay);
	};
} }

#endif
