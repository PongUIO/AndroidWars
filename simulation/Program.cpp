#include "Program.h"
#include "data/ProgramD.h"
#include "Simulation.h"

#include "program/ProgramInclude.h"

namespace Sim {
	// Program
	//
	//
	/**
	 * This implements the getTypeName() function for all
	 * internal program types.
	 */
#define _SIM_PROGRAM_DEF(baseClass,		type,	name) \
	const std::string &Prog::baseClass::getTypeName() {\
		static std::string typeName = name; \
		return typeName; \
	}
#include "program/ProgramDef.def"
#undef _SIM_PROGRAM_DEF
	
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
	{
		mLastPhaseInputId = getCurrentUniqueId();
	}
	
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
	
	const DataBehaviourT<Program>::Behaviour* ProgramFactory::getBehaviourFromId(uint32_t id) const
	{	return mSim->getData().getProgramDb().getType(id); }

	const DataBehaviourT<Program>::Behaviour* ProgramFactory::getBehaviourFromName(const std::string& name) const
	{	return mSim->getData().getProgramDb().getType(name); }

	
	
	void ProgramFactory::startPhase()
	{}
	
	void ProgramFactory::endPhase()
	{
		mLastPhaseInputId = getCurrentUniqueId();
	}
	
	void ProgramFactory::step(double stepTime)
	{}
	
	void ProgramFactory::save(Save::BasePtr& fp)
	{	fp << mLastPhaseInputId;
		UidFactory<Program>::save(fp); }
	
	void ProgramFactory::load(Save::BasePtr& fp)
	{	fp >> mLastPhaseInputId;
		UidFactory<Program>::load(fp); }
}
