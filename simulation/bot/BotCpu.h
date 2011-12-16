#ifndef SIM_BOTCPU_H
#define SIM_BOTCPU_H

#include <stdint.h>
#include <deque>
#include <queue>
#include <list>

#include "../Factory.h"

namespace Sim {
	// Forward declarations
	class Bot;
	
	class BotCpu {
		public:
			struct Schedule {
				Schedule(uint32_t progId, uint32_t activateStep,
					uint32_t scheduleCounter) :
					mProgId(progId), mActivateStep(activateStep),
					mScheduleCounter(scheduleCounter)
				{}
				
				uint32_t mProgId;
				uint32_t mActivateStep;
				uint32_t mScheduleCounter;
				
				bool isReady(uint32_t currentStep) const
				{ return currentStep >= mActivateStep; }
				
				bool operator< (const Schedule &other) const
				{ return mActivateStep<other.mActivateStep ||
					(mActivateStep==other.mActivateStep &&
					mScheduleCounter<other.mScheduleCounter); }
			};
			
			typedef std::priority_queue<Schedule> ScheduleQueue;
			typedef std::list<uint32_t> ProgramRefList;
			
			BotCpu();
			~BotCpu();
			
			void scheduleProgram(uint32_t progId, uint32_t stepDelay);
			const ProgramRefList &getProgramList() { return mProgramList; }
			
			void save(Save::BasePtr &fp);
			void load(Save::BasePtr &fp);
			
		private:
			void setHost(Bot *host) { mHost=host; }
			
			void step(double stepTime);
			void startProgram(const Schedule &sch);
			
			Bot *mHost;
			
			ScheduleQueue mScheduleQueue;
			uint32_t mScheduleCounter;
			
			ProgramRefList mProgramList;
			
			friend class Bot;
	};
};

#endif
