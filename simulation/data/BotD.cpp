#include "../collision/Collision.h"
#include "BotD.h"

namespace Sim {
	BotD::BotD() :
		baseSpeed(1.0),
		baseWeight(1.0),
		
		cpuCycleSpeed(10),
		cpuStorage(20),
		
		coreHealth(0,1),
		
		collision(NULL)
	{
	}
	
	BotD::~BotD()
	{
		if(collision)
			delete collision;
	}
	
	void BotDatabase::checksum(Save::SyncPtr& sync)
	{
		for(DataVec::iterator i=mData.begin(); i!=mData.end(); i++) {
			BotD *d = *i;
			
			sync.writeFloat(d->baseSpeed);
			sync.writeFloat(d->baseWeight);
			d->getCollision()->save(sync);
		}
	}
}
