#ifndef EXTSIM_MOVETOWARDS_H
#define EXTSIM_MOVETOWARDS_H

#include "../../object/TypeRule.h"

namespace exts {
	namespace type {
		class MoveTowards : public TypeRule {
			public:
				MoveTowards(ExtSim& esim);
				~MoveTowards();
				
				TypeRule* clone();
				void load(Nepeta::Node& node);
				
				void makeInput(const ParamList* param) const;
				void buildTimelineData(const ParamList* param) const {}
				Sim::IdType registerSimData(const std::string& name) const {}
				
				bool checkConstrained(const ParamList* param) const;
				
			private:
		};
	}
}

#endif
