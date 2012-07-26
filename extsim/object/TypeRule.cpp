#include "TypeRule.h"
#include "ParamList.h"

namespace ExtS {
	bool TypeRule::checkConstrained(const ParamList* param) const
	{
		if(mRefParam->size() != param->size())
			return false;
		
		// Compare each parameter to test against the reference parameters
		// if any parameter is not constrained, return false 
		for(size_t i=0; i<mRefParam->size(); ++i) {
			if(!mRefParam->getParam(i)->isConstrained(
			param->getParam(i), mExtSim)) {
				return false;
			}
		}
		
		return true;
	}

}
