#ifndef SIM_BOT_H
#define SIM_BOT_H

#include <stdint.h>

#include "Factory.h"
#include "Input.h"

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
			Bot(uint32_t id);
			~Bot();
			
			uint32_t mId;
			InputBuffer<BotInput> mInput;
			
			friend class BotFactory;
	};
	
	class BotFactory : public Factory<Bot> {
		public:
			BotFactory(Simulation *sim);
			~BotFactory();
			
			uint32_t createBot();
			
			void startPhase();
			void endPhase();
			
			InputBuffer<BotInput> &getInput() { return mInput; }
		private:
			void deleteInstance(Bot *obj) { delete obj; }
			
			InputBuffer<BotInput> mInput;
			
	};
	
}

#endif
