#ifndef SIM_PROGRAM_SHOOT_H
#define SIM_PROGRAM_SHOOT_H

#include "../Program.h"

namespace Sim {
	namespace Prog {
		/**
		 * Queues an order to fire a bot weapon.
		 */
		class Shoot : public Program {
			public:
				_SIM_PROGRAM_HEADER
				
				struct Config {
					uint32_t mTargetIndex;
					Save mWeapArg;
					
					explicit Config(uint32_t index=0, const Save &arg=Save()) :
						mTargetIndex(index), mWeapArg(arg) {}
				};
				
				Shoot(Simulation* sim, uint32_t id, uint32_t typeId,
					const Config &cfg);
				~Shoot();
				
				uint32_t getCycleCost();
				
			private:
				void save(Save::BasePtr& fp);
				void load(Save::BasePtr& fp);
				
				void process(Bot* bot, BotCpu* cpu);
				bool isFinished(Bot* bot, BotCpu* cpu) { return false; }
				
				uint32_t mTargetIndex;
				Save mWeapArg;
		};
	}
}

#endif
