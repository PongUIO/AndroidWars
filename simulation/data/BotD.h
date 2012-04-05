#ifndef SIM_BOTD_H
#define SIM_BOTD_H

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
	
	class BotD : public BehaviourT<Bot> {
		public:
			BotD();
			~BotD() {
				if(mCollision)
					delete mCollision;
			}
			
			Bot* createObj(Simulation* sim, IdType id) const;
			
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
			{ return mCollision; }
			
			void setCollision(Collision *c)
			{ mCollision = c; }
			
		private:
			Collision *mCollision;
	};
	
	class BotDatabase : public DataBehaviourT<Bot,BotD> {
		public:
			BotDatabase() {}
			virtual ~BotDatabase() {}
	};
}

#endif
