#include "Sensor.h"

#include "../Bot.h"

namespace Sim {
	namespace Prog {
		void Sensor::execute(Bot* bot, BotCpu* cpu)
		{
			for(ProgramIdVec::iterator i=mResponseProg.begin();
				i!=mResponseProg.end(); ++i) {
				Schedule &sch = *i;
				cpu->scheduleProgram(sch.mId, sch.mDelay);
			}
		}
		
		void Sensor::save(Save::BasePtr& fp) const
		{
			fp.writeCtr(mResponseProg);
		}
		
		void Sensor::load(Save::BasePtr& fp)
		{
			fp.readCtr(mResponseProg);
		}
	}
}
