#include "BaseData.h"

namespace ExtS {
	BaseData::~BaseData()
	{}
	
	template<>
	bool BaseData::badCastStrategy(const std::string &str, bool def)
	{
		if(str=="false")
			return false;
		else if(str=="true")
			return true;
		else
			return def;
	}
}
