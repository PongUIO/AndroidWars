#ifndef EXTSIM_MOVETOWARDS_H
#define EXTSIM_MOVETOWARDS_H

#include "../../object/TypeRule.h"

namespace exts { namespace prog {
	class MoveTowards : public TypeRule {
		public:
			MoveTowards(ExtSim& esim);
			~MoveTowards();
			
			TypeRule* clone() { return new MoveTowards(*this); }
			
			void registerInput(ParamList* param) const;
			void makeInput(const ParamList* param) const;
			void buildTimelineData(const ParamList* param) const {}
			Sim::IdType registerSimData(const std::string& name) const;
			
		private:
	};
} }

#endif
