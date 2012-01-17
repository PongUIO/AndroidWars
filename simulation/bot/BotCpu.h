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
	
	class BotCpu : private Save::OperatorImpl<BotCpu> {
		public:
			struct Schedule : private Save::OperatorImpl<Schedule> {
				Schedule(IdType progId=0, uint32_t activateStep=0,
					IdType scheduleCounter=0) :
					mProgId(progId), mActivateStep(activateStep),
					mScheduleCounter(scheduleCounter)
				{}
				
				IdType mProgId;
				uint32_t mActivateStep;
				IdType mScheduleCounter;
				
				bool isReady(uint32_t currentStep) const
				{ return currentStep >= mActivateStep; }
				
				bool operator< (const Schedule &other) const
				{ return mActivateStep>other.mActivateStep ||
					(mActivateStep==other.mActivateStep &&
					mScheduleCounter>other.mScheduleCounter); }
				
				void save(Save::BasePtr &fp) const
				{ fp << mProgId << mActivateStep << mScheduleCounter; }
				void load(Save::BasePtr &fp)
				{ fp >> mProgId >> mActivateStep >> mScheduleCounter; }
			};
			
			typedef std::priority_queue<Schedule> ScheduleQueue;
			typedef std::list<IdType> ProgramRefList;
			
			BotCpu();
			~BotCpu();
			
			int32_t getCycles() const { return mCycleCount; }
			
			void scheduleProgram(IdType progId, uint32_t stepDelay);
			const ProgramRefList &getProgramList() { return mProgramList; }
			
			bool hasRunningProgram(IdType progId);
			bool isRunning() { return mIsRunning; }
			
			void feedCycles(int32_t cycleCount);
			
			void save(Save::BasePtr &fp) const;
			void load(Save::BasePtr &fp);
			
		private:
			void initialize(Bot *host);
			
			void step(double stepTime);
			void runScheduler(double stepTime);
			void runPrograms(double stepTime);
			
			void autoSchedule(IdType progId);
			void iterProgram(bool eraseCurrent=false);
			
			/// @name Temporary data
			//@{
				Bot *mHost;
				bool mIsRunning;
				
				ProgramRefList::iterator mAutoScheduleIndex;
				
				/// Internal temporary counter that makes sure
				/// all programs are only processed once per step.
				uint32_t mMaxProcessCount;
			//@}
			
			/// @name Dynamic data
			//@{
				ScheduleQueue mScheduleQueue;
				IdType mScheduleCounter;
				
				ProgramRefList mProgramList;
				
				bool mIsEndIter;
				ProgramRefList::iterator mCurProgram;
			
				int32_t mCycleCount;
			//@}
			
			friend class Bot;
	};
};

#endif
