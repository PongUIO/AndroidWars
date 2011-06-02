#ifndef SIM_BOT_H
#define SIM_BOT_H

#include <stdint.h>

#include "Factory.h"

namespace Sim {
	
	class Bot {
		public:
			
			uint32_t getId() { return mId; }
		private:
			Bot();
			~Bot();
			
			uint32_t mId;
	};
	
	class BotFactory : public Factory<Bot> {
		public:
			BotFactory();
			~BotFactory();
			
		private:
			
	};
	
}

#endif
