#ifndef SIM_PROGRAM_SENSOR_H
#define SIM_PROGRAM_SENSOR_H

#include <stdint.h>
#include <vector>

#include "../Save.h"
#include "../Common.h"

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
		class Sensor : private Save::OperatorImpl<Sensor> {
			public:
				struct Schedule : private Save::OperatorImpl<Schedule> {
					Schedule(IdType id=0, uint32_t delay=0) :
						mId(id), mDelay(delay) {}
					
					IdType mId;
					uint32_t mDelay;
					
					void save(Save::BasePtr &fp) const
						{ fp << mId << mDelay; }
					void load(Save::BasePtr &fp)
						{ fp >> mId >> mDelay; }
				};
				typedef std::vector<Schedule> ProgramIdVec;
				
				Sensor() {}
				~Sensor() {}
				
				void insertResponse(IdType progId, uint32_t delay)
				{ mResponseProg.push_back( Schedule(progId, delay) ); }
				
				void execute(Bot *bot, BotCpu *cpu);
				
				void save(Save::BasePtr &fp) const;
				void load(Save::BasePtr &fp);
				
				bool hasResponse() const { return !mResponseProg.empty(); }
				const ProgramIdVec &getResponseVec() const
				{ return mResponseProg; }
				
			private:
				ProgramIdVec mResponseProg;
		};
	}
}

#endif
