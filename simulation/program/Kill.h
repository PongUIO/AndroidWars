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
				
				Kill(Simulation* sim, uint32_t id, const Config &cfg);
				~Kill();
				
				uint32_t getCycleCost() { return 0; }
				
			private:
				virtual void save(Save::BasePtr& fp);
				virtual void load(Save::BasePtr& fp);

				virtual void process(Bot* bot, BotCpu* cpu);
				virtual bool isFinished(Bot* bot, BotCpu* cpu) { return true; }
				
				uint32_t mTargetId;
		};
	}
}

#endif
