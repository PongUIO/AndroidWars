#include "TypeRule.h"

namespace ExtS {
	// TypeParam
	//
	//
	TypeParam::TypeParam()
	{}
	
	TypeParam::~TypeParam()
	{
		for(ParamVec::iterator i=mParam.begin(); i!=mParam.end(); ++i)
			delete *i;
	}


}