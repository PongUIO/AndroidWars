#include "Program.h"

#include "program/ProgramInclude.h"

namespace Sim {
	// Program
	//
	//
	
	/// Implement special functions for programs
#define _SIM_PROGRAM_DEF(clsType) \
	uint32_t Prog::clsType::getTypeId() { return Pti##clsType; } \
	Program * Prog::clsType::SaveSys::createProgram(Simulation *sim, \
		uint32_t id) { return new clsType(sim, id, Config()); }
	
	#include "program/ProgramDef.def"
#undef _SIM_PROGRAM_DEF
	
	// ProgramFactory
	//
	//
	ProgramFactory::ProgramFactory(Simulation* sim)
		: Factory<Program>(), mSim(sim), mCurrentId(0)
	{
		using namespace Prog;
		
		// Register all program types
		#define _SIM_PROGRAM_DEF(clsType) \
		mTypeMap[clsType::getTypeId()] = new clsType::SaveSys();
		
		#include "program/ProgramDef.def"
		
		#undef _SIM_PROGRAM_DEF
	}
	
	ProgramFactory::~ProgramFactory()
	{}
	
	void ProgramFactory::startup()
	{}
	
	void ProgramFactory::shutdown()
	{
		killAll();
		
		for(ProgramTypeMap::iterator i=mTypeMap.begin(); i!=mTypeMap.end();
			i++) {
			delete i->second;
		}
	}
	
	
	void ProgramFactory::startPhase()
	{}
	
	void ProgramFactory::endPhase()
	{}
	
	void ProgramFactory::step(double stepTime)
	{}
	
	void ProgramFactory::save(Save::BasePtr& fp)
	{
		fp.writeInt<uint32_t>(mCurrentId);
		
		Factory<Program>::save(fp);
	}
	
	void ProgramFactory::load(Save::BasePtr& fp)
	{
		mCurrentId = fp.readInt<uint32_t>();
		
		Factory<Program>::load(fp);
	}
	
	void ProgramFactory::saveObj(Program *obj, Save::BasePtr&fp)
	{
		fp.writeInt<uint32_t>(obj->getTypeId());
		fp.writeInt<uint32_t>(obj->getId());
		
		obj->save(fp);
	}
	
	Program* ProgramFactory::loadObj(uint32_t internalId, Save::BasePtr &fp)
	{
		uint32_t progType = fp.readInt<uint32_t>();
		uint32_t progId = fp.readInt<uint32_t>();
		
		Program *prog = mTypeMap[progType]->createProgram(mSim, progId);
		prog->load(fp);
		prog->mInternalFactoryId = internalId;
		
		insertResolve(progId, prog);
		
		return prog;
	}

}
