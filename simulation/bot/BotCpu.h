#ifndef SIM_BOTCPU_H
#define SIM_BOTCPU_H

#include <stdint.h>
#include <deque>

#include "../Factory.h"

namespace Sim {
	// Forward declarations
	class Bot;
	
	class BotCpu {
		public:
			struct Schedule {
				Schedule(uint32_t type, uint32_t delay) :
					mType(type), mDelay(delay) {}
				
				uint32_t mType;
				uint32_t mDelay;
			};
			typedef std::deque<Schedule> ScheduleQueue;
			
			struct Program {
				uint32_t mId;
			};
			
			
			BotCpu(Bot *host);
			~BotCpu();
			
			void scheduleProgram(uint32_t typeId, uint32_t stepDelay);
			
		private:
			void step(double stepTime);
			void startProgram(Schedule &sch);
			
			Bot *mHost;
			ScheduleQueue mSchedule;
			
			friend class Bot;
	};
};

#endif
