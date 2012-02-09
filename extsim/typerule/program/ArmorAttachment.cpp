#include "../../ExtSim.h"
#include "ArmorAttachment.h"

#include "../../param/IdList.h"
#include "../../param/ValRange.h"

namespace ExtS { namespace Prog {
	ArmorAttachmentRule::ArmorAttachmentRule()
	{
		registerMetaParam(new MetaIdList<Sim::ArmorD>("ArmorType"));
		registerMetaParam(new MetaValRange<uint32_t>("Health"));
	}
	
	void ArmorAttachmentRule::saveInput(ParamList *param,
		Sim::Save::BasePtr&fp, ExtSim &esim ) const
	{
		/*Sim::Program *p = esim.getSim().getState().getProgramFactory().
			createType<Sim::Prog::>();*/
	}

} }
