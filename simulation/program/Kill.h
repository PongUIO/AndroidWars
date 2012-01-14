#ifndef SIM_PROGRAM_KILL_H
#define SIM_PROGRAM_KILL_H

#include "../Program.h"

namespace Sim {
	namespace Prog {
		/**
		 * Kills an active program on a bot cpu. The ID given to this program
		 * must be currently running and on the same cpu as the kill program,
		 * if not it does nothing.
		 */
		class Kill : public Program {
			public:
				_SIM_PROGRAM_HEADER
				
				struct Config {
					uint32_t mTargetId;
					
					explicit Config(uint32_t id=FactoryNoId) : mTargetId(id) {}
				};
				
				Kill(Simulation* sim, uint32_t id, uint32_t typeId,
					 const Config &cfg);
				~Kill();
				
				uint32_t getCycleCost() { return 0; }
				
			private:
				void save(Save::BasePtr& fp);
				void load(Save::BasePtr& fp);

				void process(Bot* bot, BotCpu* cpu);
				bool isFinished(Bot* bot, BotCpu* cpu) { return false; }
				
				uint32_t mTargetId;
		};
	}
}

#endif
