#include "Kill.h"

#include "../Bot.h"
#include "../bot/BotCpu.h"
#include "../Simulation.h"

namespace Sim {
	namespace Prog {
		Kill::Kill(Simulation* sim, IdType id, IdType typeId,
			const Config &cfg) :
			Program(sim, id, typeId),
			mTargetId(cfg.mTargetId)
		{}
		
		Kill::~Kill()
		{}
		
		void Kill::process(Bot* bot, BotCpu* cpu)
		{
			if(cpu->hasRunningProgram(mTargetId)) {
				Program *p = mSim->getState().getProgramFactory().
					getProgram(mTargetId);
				if(p)
					p->setForceFinished();
			}
			
			// Kill always runs once
			setForceFinished();
		}
		
		void Kill::save(Save::BasePtr& fp)
		{
			fp << mTargetId;
			
			return Program::save(fp);
		}
		
		void Kill::load(Save::BasePtr& fp)
		{
			fp >> mTargetId;
			
			return Program::load(fp);
		}
	}
}
