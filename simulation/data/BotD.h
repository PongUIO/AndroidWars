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
	
	struct BotD {
		BotD();
		~BotD();
		
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
		{ return collision; }
		
		private:
			uint32_t mId;
			
			Collision *collision;
			
			friend class BotDatabase;
			friend class DataCtr<BotD>;
	};
	
	class BotDatabase : public DataT<BotD> {
		public:
			BotDatabase() {}
			virtual ~BotDatabase() {}
			
			/// @name Compatibility layer
			//@{
				const BotD &getBot(IdType type) const
				{ return *getDataById(type); }
				
				IdType addBot(const BotD &bot, const Collision::ColPoints &pts)
				{
					IdType id = addType(new BotD(bot) );
					BotD &newBot = *rawGet(id);
					newBot.collision = new Collision(pts);
					
					return id;
				}
				
				template<class T>
				DataBehaviourT<Bot>::Behaviour *getType(T val) { return 0; }
			//@}
			
			void checksum(Save::SyncPtr &sync);
	};
}

#endif
