#include "BotCpu.h"
#include "../Bot.h"
#include "../Simulation.h"

namespace Sim {
	// BotCpu
	//
	//
	BotCpu::BotCpu() : mHost(0),
			mScheduleQueue(), mScheduleCounter(0),
			mProgramList()
		{}
	
	BotCpu::~BotCpu()
		{}
	
	void BotCpu::scheduleProgram(uint32_t typeId, uint32_t stepDelay)
	{
		mScheduleQueue.push(
			Schedule(
				typeId,
				mHost->mSim->getCurTotalStep()+stepDelay,
				mScheduleCounter++
			)
		);
	}
	
	void BotCpu::startProgram(const Sim::BotCpu::Schedule& sch)
	{
		mProgramList.push_back(sch.mProgId);
	}
	
	void BotCpu::step(double stepTime)
	{
		// Update the scheduler
		while(!mScheduleQueue.empty()) {
			const Schedule &sch = mScheduleQueue.top();
			
			if(sch.isReady(mHost->mSim->getCurTotalStep())) {
				startProgram(sch);
				mScheduleQueue.pop();
			}
			else
				break;
		}
		
		// Run programs
		for(ProgramRefList::iterator i=mProgramList.begin();
			i!=mProgramList.end();) {
			Program *prog = mHost->mSim->getState().
				getProgramFactory().getProgram(*i);
			
			bool doErase = true;
			if(prog) {
				prog->process(mHost, this);
				if(!prog->isFinished(mHost, this))
					doErase = false;
			}
				
			if(doErase)
				i = mProgramList.erase(i);
			else
				i++;
		}
	}
	
	/**
	 * Checks if the CPU has a running program with the given ID.
	 */
	bool BotCpu::hasRunningProgram(uint32_t progId)
	{
		for(ProgramRefList::iterator i=mProgramList.begin();
			i!=mProgramList.end(); i++) {
			if(*i == progId)
				return true;
		}
		
		return false;
	}
	
	void BotCpu::save(Save::BasePtr& fp)
	{
		
		ScheduleQueue scheduleCopy = ScheduleQueue(mScheduleQueue);
		fp.writeInt<uint32_t>(scheduleCopy.size());
		while(scheduleCopy.size()>0) {
			const Schedule &sch = scheduleCopy.top();
			fp.writeInt<uint32_t>(sch.mProgId);
			fp.writeInt<uint32_t>(sch.mActivateStep);
			fp.writeInt<uint32_t>(sch.mScheduleCounter);
			scheduleCopy.pop();
		}
		fp.writeInt<uint32_t>(mScheduleCounter);
		
		fp.writeInt<uint32_t>(mProgramList.size());
		for(ProgramRefList::iterator i=mProgramList.begin();
			i!=mProgramList.end(); i++) {
			fp.writeInt<uint32_t>(*i);
		}
	}
	
	void BotCpu::load(Save::BasePtr& fp)
	{
		uint32_t loadCount;
		
		mScheduleQueue = ScheduleQueue();
		loadCount = fp.readInt<uint32_t>();
		while(loadCount>0) {
			uint32_t progId = fp.readInt<uint32_t>();
			uint32_t activateStep = fp.readInt<uint32_t>();
			uint32_t schCount = fp.readInt<uint32_t>();
			
			mScheduleQueue.push( Schedule(progId, activateStep, schCount) );
			loadCount--;
		}
		mScheduleCounter = fp.readInt<uint32_t>();
		
		mProgramList.clear();
		loadCount = fp.readInt<uint32_t>();
		while(loadCount>0) {
			uint32_t progId = fp.readInt<uint32_t>();
			mProgramList.push_back(progId);
			loadCount--;
		}
	}
	
}
