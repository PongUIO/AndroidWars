#include "BotCpu.h"
#include "../Bot.h"
#include "../Simulation.h"

namespace Sim {
	// BotCpu
	//
	//
	BotCpu::BotCpu() :
			mHost(0), mIsRunning(false),
			mAutoScheduleIndex(mProgramList.end()),
			mMaxProcessCount(0),
			
			mScheduleQueue(), mScheduleCounter(0),
			mProgramList(),
			mIsEndIter(true), 
			mCurProgram(mProgramList.end()),
			mCycleCount(0)
		{}
	
	BotCpu::~BotCpu()
		{}
	
	void BotCpu::scheduleProgram(IdType progId, uint32_t stepDelay)
	{
		if(mIsRunning && stepDelay==0) {
			autoSchedule(progId);
		} else {
			mScheduleQueue.push(
				Schedule(
					progId,
					mHost->mSim->getCurTotalStep()+stepDelay,
					mScheduleCounter++
				)
			);
		}
	}
	
	void BotCpu::iterProgram(bool eraseCurrent)
	{
		if(mIsEndIter) {
			mCurProgram=mProgramList.begin();
			mIsEndIter = false;
		} else {
			if(eraseCurrent)
				mCurProgram = mProgramList.erase(mCurProgram);
			else
				++mCurProgram;
			
			if(mCurProgram == mProgramList.end())
				mCurProgram = mProgramList.begin();
		}
		
		if(mCurProgram == mProgramList.end()) {
			mIsEndIter = true;
			
			// The next program will be the first program
			mAutoScheduleIndex = mProgramList.begin();
			if(mProgramList.size()!=0)
				++mAutoScheduleIndex;
		} else {
			// Schedule just after the current program
			mAutoScheduleIndex = mCurProgram;
			++mAutoScheduleIndex;
		}
	}
	
	void BotCpu::initialize(Bot* host)
	{
		mHost = host;
	}
	
	void Sim::BotCpu::feedCycles(int32_t cycleCount)
	{
		int32_t newCycles = mCycleCount+cycleCount;
		
		if(newCycles < 0)
			newCycles = 0;
		else if(newCycles > int32_t(mHost->getTypePtr()->cpuStorage))
			newCycles = mHost->getTypePtr()->cpuStorage;
		
		mCycleCount = newCycles;
	}
	
	/**
	 * Performs a single step on the CPU.
	 */
	void BotCpu::step(double stepTime)
	{
		const BotD *typePtr = mHost->getTypePtr();
		
		// Add cycles to the CPU
		feedCycles(typePtr->cpuCycleSpeed);
		
		runScheduler(stepTime);
		runPrograms(stepTime);
	}
	
	/**
	 * Updates the program scheduler.
	 * 
	 * The scheduler adds scheduled programs to the list of currently
	 * running programs when they are ready.
	 * 
	 * Programs scheduled by the CPU are, at present, always added so that they
	 * are the next program to be run (they displace any other running copy).
	 * If several programs are added at the same time, they are added in
	 * the order they were scheduled.
	 * 
	 * This system is in place to allow a CPU to kill the currently active
	 * program if it consumes too many cycles.
	 */
	void BotCpu::runScheduler(double stepTime)
	{
		ProgramRefList::iterator head;
		if(mIsEndIter)
			head = mProgramList.end();
		else {
			head = mCurProgram;
		}
		
		// Update the scheduler
		bool hasSetCurrent = false;
		while(!mScheduleQueue.empty()) {
			const Schedule &sch = mScheduleQueue.top();
			
			if(sch.isReady(mHost->mSim->getCurTotalStep())) {
				ProgramRefList::iterator newIter =
					mProgramList.insert(head, sch.mProgId);
				if(!hasSetCurrent) {
					mCurProgram=newIter;
					mIsEndIter=false;
					hasSetCurrent = true;
				}
				mScheduleQueue.pop();
			}
			else
				break;
		}
	}
	
	/**
	 * Schedules a new program while the CPU is running.
	 * 
	 * This bypasses the scheduler and immediately adds a new running program.
	 * The programs scheduled like this will be the next running programs in
	 * sequence after the active program is finished.
	 * 
	 * This triggers any \c Program::start() if the program has one, and
	 * also updates the number of processes that may be processed this
	 * step.
	 */
	void BotCpu::autoSchedule(IdType progId)
	{
		mProgramList.insert(mAutoScheduleIndex, progId);
		mMaxProcessCount++;
		
		Program *prog = mHost->mSim->getState().getProgramFactory().
			getProgram(progId);
		prog->start(mHost, this);
	}
	
	/**
	 * Runs programs on the CPU.
	 * 
	 * Programs are executed in order, but any program will only execute once
	 * per step. This means that if the CPU has enough cycles in total for
	 * all the programs, all of them will be run.
	 * 
	 * If there is a lack of cycles for the next program, then the CPU will
	 * abort processing. The CPU will never skip a program in order to
	 * exhaust as many cycles as possible.
	 */
	void BotCpu::runPrograms(double stepTime)
	{
		// Run programs
		if(mIsEndIter)
			iterProgram();
		
		mMaxProcessCount = mProgramList.size();
		if(mCurProgram != mProgramList.end()) {
			// Flag the CPU as running its main loop
			mIsRunning = true;
			
			do {
				Program *prog = mHost->mSim->getState().getProgramFactory().
					getProgram(*mCurProgram);
				
				bool doErase = false;
				bool doAbort = false;
				
				if(prog) {
					// Abort the loop if the CPU can't afford to run the next
					// program
					if(uint32_t(mCycleCount) >= prog->getCycleCost()) {
						// Note: The CPU immediately eat the cycles
						// in case the program yields a different cycle
						// cost after it has processed.
						//
						// Though it is just a theoretical possibility,
						// it will probably not occur in practice.
						mCycleCount -= prog->getCycleCost();
						
						if(!prog->isCompletelyFinished(mHost, this))
							prog->process(mHost, this);
						
						prog->tickRunningTime();
						
						if(prog->isCompletelyFinished(mHost,this)) {
							prog->end(mHost, this);
							doErase = true;
						}
					} else
						doAbort = true;
					
				} else {
					doErase = true;
				}
				
				if(doAbort)
					break;
				
				iterProgram(doErase);
			} while( (--mMaxProcessCount) > 0 );
			
			// Flag the CPU as not running its main loop
			mIsRunning = false;
		}
	}
	
	/**
	 * Checks if the CPU has a running program with the given ID.
	 */
	bool BotCpu::hasRunningProgram(IdType progId)
	{
		for(ProgramRefList::iterator i=mProgramList.begin();
			i!=mProgramList.end(); ++i) {
			if(*i == progId)
				return true;
		}
		
		return false;
	}
	
	void BotCpu::save(Save::BasePtr& fp) const
	{
		fp.writeQueue(mScheduleQueue);
		fp << mScheduleCounter;
		
		fp.writeCtr(mProgramList);
		
		fp << mIsEndIter;
		if(mIsEndIter) {
			// Do nothing.
		} else {
			ProgramRefList::const_iterator curIter= mCurProgram;
			fp << uint32_t(std::distance(mProgramList.begin(),curIter));
		}
		
		fp << mCycleCount;
	}
	
	void BotCpu::load(Save::BasePtr& fp)
	{
		fp.readQueue(mScheduleQueue);
		fp >> mScheduleCounter;
		
		fp.readCtr(mProgramList);
		
		fp >> mIsEndIter;
		if(mIsEndIter)
			mCurProgram = mProgramList.end();
		else {
			uint32_t iterCount;
			fp >> iterCount;
			mCurProgram = mProgramList.begin();
			std::advance(mCurProgram, iterCount);
		}
		
		fp >> mCycleCount;
	}
	
}
