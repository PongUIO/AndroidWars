#ifndef SIM_BOT_H
#define SIM_BOT_H

#include <stdint.h>

#include "Factory.h"
#include "Input.h"
#include "Body.h"

namespace Sim {
	// Forward declarations
	class Simulation;
	class BotFactory;
	
	class Bot {
		public:
			bool isDead() { return false; }
			void step(double stepTime);
			uint32_t getId() { return mId; }
			
		private:
			Bot(uint32_t id, uint32_t side);
			~Bot();
			
			uint32_t mId;
			uint32_t mSide;
			InputBuffer<BotInput> mInput;
			
			Body mBody;
			
			friend class BotFactory;
	};
	
	class BotFactory : public Factory<Bot> {
		public:
			BotFactory(Simulation *sim);
			~BotFactory();
			
			uint32_t createBot(uint32_t side);
			
			void startPhase();
			void endPhase();
			
			InputBuffer<BotInput> &getInput() { return mInput; }
		private:
			void deleteInstance(Bot *obj) { delete obj; }
			
			InputBuffer<BotInput> mInput;
			
	};
	
}

#endif
