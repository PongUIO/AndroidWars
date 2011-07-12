#include "../collision/Collision.h"
#include "BotD.h"

namespace Sim {
	BotD::BotD() :
		collision(NULL),
		baseSpeed(1.0),
		baseWeight(1.0)
	{
	}
	
	BotD::~BotD()
	{
		if(collision)
			delete collision;
	}
	
	void BotDatabase::shutdown()
	{
		mData.clear();
	}

	void BotDatabase::checksum(Save::SyncPtr& sync)
	{
		for(BotVec::iterator i=mData.begin(); i!=mData.end(); i++) {
			BotD &d = *i;
			
			sync.writeFloat(d.baseSpeed);
			sync.writeFloat(d.baseWeight);
			d.getCollision()->save(sync);
		}
	}
}
