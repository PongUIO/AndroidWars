#ifndef EXTSIM_PROGRAM_MOVETOWARDS_H
#define EXTSIM_PROGRAM_MOVETOWARDS_H

#include "../../TypeRule.h"

namespace ExtS { namespace Prog {
	class MoveTowardsRule : public TypeRule {
		public:
			MoveTowardsRule();
			virtual ~MoveTowardsRule() {}
			
			virtual TypeRule* clone()
			{ return new MoveTowardsRule(*this); }
			
			virtual Sim::IdType registerSimData(ExtSim& esim,
				const std::string& name);
			
			virtual void makeInput(ExtSim &extsim,
				Sim::IdType simTypeId, const ParamList* param) const;
	};
} }

#endif
