#ifndef SIM_BOTD_H
#define SIM_BOTD_H

#include <boost/shared_ptr.hpp>

#include <vector>
#include <stdint.h>
#include "../Save.h"
#include "../collision/Collision.h"

#include "BaseData.h"
#include "../bot/BotAbility.h"
#include "../Health.h"

namespace Sim {
	class Collision;
	class BotDatabase;
	class Bot;
	
	class BotD {
		public:
			BotD();
			~BotD() {}
			
			double baseSpeed;
			double baseWeight;
			
			uint32_t cpuCycleSpeed;
			uint32_t cpuStorage;
			
			Health coreHealth;
			
			/// Programs that this bot has
			BotAbility::AvailableProgram baseProgram;
			
			/// Programs that this bot is excluded from running.
			/// Overrides both \c baseProgram and \c Player::mBasePrograms
			BotAbility::AvailableProgram excludeProgram;
			
			const Collision *getCollision() const
			{ return mCollision.get(); }
			
			void setCollision(Collision *c)
			{ mCollision = CollisionPtr(c); }
			
		private:
			typedef boost::shared_ptr<Collision> CollisionPtr;
			
			CollisionPtr mCollision;
	};
	
	class BotDatabase : public DefaultBehaviourDatabase<Bot,BotD> {
		public:
			BotDatabase() {}
			virtual ~BotDatabase() {}
	};
}

#endif
