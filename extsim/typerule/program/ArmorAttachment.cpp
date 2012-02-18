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

	
	/*void ArmorAttachmentRule::saveInput(ParamList *param,
		Sim::Save::BasePtr&fp, ExtSim &esim ) const
	{
		Sim::Program *p = esim.getSim().getState().getProgramFactory().
			createType<Sim::Prog::>();
	}*/

} }
