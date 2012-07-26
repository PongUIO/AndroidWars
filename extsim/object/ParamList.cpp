#include "ParamList.h"

namespace ExtS {
	void ParamList::save(Sim::Save::BasePtr& fp) const
	{
		fp << mRuleParam;
	}

	void ParamList::load(Sim::Save::BasePtr& fp)
	{
		fp >> mRuleParam;
	}
}
