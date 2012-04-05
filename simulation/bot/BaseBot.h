#ifndef SIM_BOT_BASEBOT_H
#define SIM_BOT_BASEBOT_H

#include "../Bot.h"

namespace Sim {
	class BaseBot : public Bot {
		public:
			const std::string &getTypeName() {
				static std::string typeName = "Base";
				return typeName;
			}
			
			BaseBot(
	};
}

#endif
