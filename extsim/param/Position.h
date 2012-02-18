#ifndef EXTSIM_POSITION_H
#define EXTSIM_POSITION_H

#include "../../simulation/Vector.h"

#include "../TypeRule.h"
#include "../ExtData.h"

#include "ValRange.h"

namespace ExtS {
	/**
	 * Used to denote a world position.
	 */
	class PositionParam : public ValRange<Sim::Vector>,
	public ListenerSlot<PositionParam> {
		public:
			PositionParam(const std::string& dataName) :
				ValRange<Sim::Vector>(dataName) {}
			virtual ~PositionParam() {}
			
			virtual RuleParameter* clone()
			{ return new PositionParam(*this); }
			
			virtual void callback()
			{ ListenerSlot<PositionParam>::raiseListener(this); }
			
		private:
	};
}

#endif
