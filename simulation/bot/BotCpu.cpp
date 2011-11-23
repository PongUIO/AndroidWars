#include "BotCpu.h"

namespace Sim {
	
	BotCpu::BotCpu(Bot* host) : mHost(host)
		{}
	
	BotCpu::~BotCpu()
		{}
	
	void BotCpu::scheduleProgram(uint32_t typeId, uint32_t stepDelay)
	{
		mSchedule.push_back( Schedule(typeId, stepDelay) );
	}

	void BotCpu::step(double stepTime)
	{
		// Update the scheduler
		bool hasTick = true;
		while(!mSchedule.empty()) {
			Schedule &sch = mSchedule.front();
			
			if(sch.mDelay>0 && hasTick) {
				sch.mDelay--;
				hasTick=false;
			}
			
			if(sch.mDelay == 0) {
				startProgram(sch);
				mSchedule.pop_front();
			} else {
				break;
			}
		}//while(Schedule)
		
		// Step as many programs as possible
	}
};
