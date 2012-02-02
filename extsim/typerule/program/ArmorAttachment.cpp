#include "ArmorAttachment.h"

#include "../../param/IdList.h"
#include "../../param/ValRange.h"

namespace ExtS { namespace Prog {
	ArmorAttachmentRule::ArmorAttachmentRule()
	{
		registerMetaParam(new MetaIdList<Sim::ArmorD>("ArmorType"));
		registerMetaParam(new MetaValRange<uint32_t>("Health"));
	}
} }
