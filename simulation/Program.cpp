#include "Program.h"

#include "program/ProgramInclude.h"

namespace Sim {
	// Program
	//
	//
	
	/// Implement getTypeId() for all program types
#define _SIM_PROGRAM_DEF(clsType) \
	uint32_t Prog::clsType::getTypeId() { return Pti##clsType; }
	#include "program/ProgramDef.def"
#undef _SIM_PROGRAM_DEF
	
	// ProgramFactory
	//
	//
	ProgramFactory::ProgramFactory(Simulation* sim)
		: Factory<Program>(), mSim(sim)
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
		Factory<Program>::save(fp);
	}
	
	void ProgramFactory::load(Save::BasePtr& fp)
	{
		Factory<Program>::load(fp);
	}
	
	void ProgramFactory::saveObj(Program *obj, Save::BasePtr&fp)
	{
		fp.writeInt<uint32_t>(obj->getTypeId());
		obj->save(fp);
	}
	
	Program* ProgramFactory::loadObj(uint32_t id, Save::BasePtr &fp)
	{
		uint32_t progType = fp.readInt<uint32_t>();
		
		Program *prog = mTypeMap[progType]->createProgram(mSim, id);
		prog->load(fp);
		
		return prog;
	}
}
