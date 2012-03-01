#include "../../ExtSim.h"
#include "ArmorAttachment.h"

#include "../../param/IdList.h"
#include "../../param/ValRange.h"

namespace ExtS { namespace Prog {
	ArmorAttachmentRule::ArmorAttachmentRule()
	{
		registerRuleParam(new IdList<Sim::ArmorD>("ArmorType"));
		registerRuleParam(new ValRange<uint32_t>("Health"));
	}

	bool ArmorAttachmentRule::makeInput(
		Sim::Save::BasePtr& fp, const ParamList* param) const
	{
		return true;
	}
} }
