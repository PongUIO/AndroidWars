#ifndef EXTSIM_PROGRAM_MOVETOWARDS_H
#define EXTSIM_PROGRAM_MOVETOWARDS_H

#include "../../TypeRule.h"

namespace ExtS { namespace Prog {
	class MoveTowardsRule : public TypeRule {
		public:
			MoveTowardsRule();
			~MoveTowardsRule() {}
			
			virtual TypeRule* clone()
			{ return new MoveTowardsRule(*this); }
			
			virtual void makeInput(ExtSim &extsim, const ParamList* param) const;
			
		private:
	};
} }

#endif
