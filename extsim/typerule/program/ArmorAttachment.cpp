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

	void ArmorAttachmentRule::makeInput(
		ExtSim& extsim, const ParamList* param) const
	{
	}
} }
