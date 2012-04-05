#ifndef SIM_BOT_BASEBOT_H
#define SIM_BOT_BASEBOT_H

#include "../Bot.h"

namespace Sim {
	class BaseBot : public Bot {
		public:
			static const std::string &getTypeName() {
				static std::string typeName = "Base";
				return typeName;
			}
			
		BaseBot(Simulation* sim, IdType id, IdType typeId,
			const Bot::State& cfg = State()) : Bot(sim,id,typeId,cfg) {}
		~BaseBot() {}
	};
}

#endif
