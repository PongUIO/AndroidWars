#ifndef SIM_PROGRAM_SENSOR_H
#define SIM_PROGRAM_SENSOR_H

#include <stdint.h>
#include <vector>

#include "../Save.h"

namespace Sim {
	class Bot;
	class BotCpu;
	
	namespace Prog {
		/**
		 * Base program for bot sensors. These are designed to schedule a
		 * set of programs to a bot when an event occurs.
		 * 
		 * The class is not a true program by itself, it should be utilized
		 * by other program types to simplify mass scheduling of programs.
		 */
		class Sensor {
			public:
				struct Schedule {
					Schedule(uint32_t id, uint32_t delay) :
						mId(id), mDelay(delay) {}
					
					uint32_t mId;
					uint32_t mDelay;
				};
				typedef std::vector<Schedule> ProgramIdVec;
				
				Sensor() {}
				~Sensor() {}
				
				void insertResponse(uint32_t progId, uint32_t delay)
				{ mResponseProg.push_back( Schedule(progId, delay) ); }
				
				void execute(Bot *bot, BotCpu *cpu);
				
				void save(Save::BasePtr &fp);
				void load(Save::BasePtr &fp);
				
				bool hasResponse() { return !mResponseProg.empty(); }
				const ProgramIdVec &getResponseVec() { return mResponseProg; }
				
			private:
				ProgramIdVec mResponseProg;
		};
	}
}

#endif
