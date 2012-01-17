#include "ArmorAttachment.h"

#include "../Bot.h"

namespace Sim { namespace Abil {
	ArmorAttachment::ArmorAttachment(Simulation* sim, IdType id,
		IdType typeId, const Config& cfg) : Ability(sim, id, typeId), mData(cfg)
	{}
	
	ArmorAttachment::~ArmorAttachment()
	{}
	
	void ArmorAttachment::start(Bot* bot)
	{
		bot->getHealth().addAttachment(mData);
	}
	
	void ArmorAttachment::end(Bot* bot)
	{
		bot->getHealth().removeAttachment(mData);
	}
	
	void ArmorAttachment::save(Save::BasePtr& fp)
	{
		fp << mData;
	}

	void ArmorAttachment::load(Save::BasePtr& fp)
	{
		fp >> mData;
	}
} }
