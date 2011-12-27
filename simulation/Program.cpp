#include "Program.h"
#include "data/ProgramD.h"
#include "Simulation.h"

#include "program/ProgramInclude.h"

namespace Sim {
	// ProgramFactory
	//
	//
	ProgramFactory::ProgramFactory(Simulation* sim)
		: UidFactory<Program>(), mSim(sim)
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
	
	/**
	 * Special support function for program template creation.
	 * This translates a program type name into the internal ID for that type.
	 */
	uint32_t ProgramFactory::getProgramTypeId(const std::string& name)
	{ return mSim->getData().getProgramDb().getTypeIdOf(name); }
	
	
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
	
	void ProgramFactory::saveObj(Program *obj, Save::BasePtr&fp)
	{
		fp.writeInt<uint32_t>(obj->getTypeId());
		obj->save(fp);
	}
	
	Program* ProgramFactory::loadObj(uint32_t internalId, Save::BasePtr &fp)
	{
		uint32_t progType = fp.readInt<uint32_t>();
		
		const ProgramD *progData =
			mSim->getData().getProgramDb().getProgramType(progType);
		
		Program *prog = progData->createProgram(mSim, internalId);
		prog->load(fp);
		
		return prog;
	}
}
