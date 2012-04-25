#include "Program.h"
#include "data/ProgramD.h"
#include "Simulation.h"

#include "program/ProgramInclude.h"

namespace Sim {
	// Program
	//
	//
	/**
	 * Called just as a program is added to the CPU of a bot.
	 */
	void Program::start(Bot* bot, BotCpu* cpu)
	{}
	
	/**
	 * Called when a program is removed from a bot's CPU.
	 * 
	 * This is only called by the bot's CPU, if the program is removed
	 * any other way then this is not called.
	 */
	void Program::end(Bot* bot, BotCpu* cpu)
	{
		mEndSensor.execute(bot,cpu);
	}
	
	void Program::save(Save::BasePtr& fp)
	{	fp << mEndSensor << mRunningTime << mFinished; }
	
	void Program::load(Save::BasePtr& fp)
	{	fp >> mEndSensor >> mRunningTime >> mFinished; }
	
	
	// ProgramFactory
	//
	//
	ProgramFactory::ProgramFactory(Simulation* sim)
		: DefaultUidFactory<Program>(sim)
	{
	}
	
	ProgramFactory::~ProgramFactory()
	{}
	
	void ProgramFactory::startup()
	{}
	
	void ProgramFactory::shutdown()
	{
		killAll();
	}
	
	Program* ProgramFactory::createFromSerialized(Save::BasePtr& fp)
	{
		InsertData insData = insertObject();
		
		Program *prog = loadObj(insData.first, fp);
		*insData.second = prog;
		
		return prog;
	}
	
	void ProgramFactory::startPhase()
	{}
	
	void ProgramFactory::endPhase()
	{}
	
	void ProgramFactory::step(double stepTime)
	{}
	
	void ProgramFactory::save(Save::BasePtr& fp)
	{	UidFactory<Program>::save(fp); }
	
	void ProgramFactory::load(Save::BasePtr& fp)
	{	UidFactory<Program>::load(fp); }
}
