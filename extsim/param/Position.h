#ifndef EXTSIM_POSITION_H
#define EXTSIM_POSITION_H

#include "../../simulation/Vector.h"

#include "../object/ParamList.h"
#include "../ExtData.h"

#include "ValueParam.h"

namespace exts {
	/**
	 * Used to denote a world position.
	 */
	class PositionParam : public ValueParam<Sim::Vector> {
		public:
			PositionParam(const std::string& dataName) :
				ValueParam<Sim::Vector>(dataName) {}
			virtual ~PositionParam() {}
			
			virtual RuleParameter* clone()
			{ return new PositionParam(*this); }
			virtual void accept(ParamVisitor& visitor)
			{	visitor.visit(*this); }
			
		private:
	};
}

#endif
