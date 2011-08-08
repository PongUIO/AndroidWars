#ifndef SIM_BOTD_H
#define SIM_BOTD_H

#include <vector>
#include <stdint.h>
#include "../Save.h"
#include "../collision/Collision.h"

#include "BaseData.h"

namespace Sim {
	class Collision;
	class BotDatabase;
	
	struct BotD {
		BotD();
		~BotD();
		
		double baseSpeed;
		double baseWeight;
		
		const Collision *getCollision() const
		{ return collision; }
		
		private:
			Collision *collision;
			
			friend class BotDatabase;
	};
	
	class BotDatabase : public DataT<BotD> {
		public:
			BotDatabase() {}
			virtual ~BotDatabase() {}
			
			/// @name Compatibility layer
			//@{
				const BotD &getBot(uint32_t type) const
				{ return *getType(type); }
				
				uint32_t addBot(const BotD &bot, const Collision::ColPoints &pts)
				{
					uint32_t id = addType(new BotD(bot) );
					BotD &newBot = *rawGet(id);
					newBot.collision = new Collision(pts);
					
					return id;
				}
			//@}
			
			void checksum(Save::SyncPtr &sync);
	};
}

#endif
