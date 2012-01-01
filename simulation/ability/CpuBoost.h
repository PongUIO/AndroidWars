#ifndef SIM_ABILITY_CPUBOOST_H
#define SIM_ABILITY_CPUBOOST_H

#include "../Ability.h"

namespace Sim {
	namespace Abil {
		/**
		 * Alters the effective speed of the CPU.
		 * 
		 * This may provide both a percentage and absolute boost, as well
		 * as a CPU slowdown of either kind.
		 */
		class CpuBoost : public Ability {
			public:
				_SIM_ABILITY_HEADER
				
				struct Config {
					double mPercentage;
					int32_t mAbsolute;
					
					Config(double percentage=0.0, uint32_t absolute=0) :
						mPercentage(percentage), mAbsolute(absolute) {}
				};
				
				CpuBoost(Simulation* sim, uint32_t id, uint32_t typeId,
					const Config &cfg);
				virtual ~CpuBoost();
				
			private:
				void save(Save::BasePtr& fp);
				void load(Save::BasePtr& fp);
				
				void prepareStep(double delta, Bot *bot);
				void updateCpu(double delta, Bot *bot);
				void step(double delta, Bot *bot);
				
				Config mData;
		};
	}
}

#endif
