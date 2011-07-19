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
				{ return getType(type); }
				
				uint32_t addBot(const BotD &bot, const Collision::ColPoints &pts)
				{	mData.push_back(bot);
					BotD &newBot = mData.back();
					newBot.collision = new Collision(pts);
				
					return addType(newBot);
				}
			//@}
			
			void checksum(Save::SyncPtr &sync);
			
		private:
			typedef std::vector<BotD> BotVec;
			
			BotVec mData;
	};
}

#endif
