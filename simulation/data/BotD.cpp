#include "../collision/Collision.h"
#include "../Bot.h"
#include "BotD.h"

namespace Sim {
	BotD::BotD() :
		baseSpeed(1.0),
		baseWeight(1.0),
		
		cpuCycleSpeed(10),
		cpuStorage(20),
		
		coreHealth(0,1),
		
		mCollision()
	{
	}
}
