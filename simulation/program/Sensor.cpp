#include "Sensor.h"

#include "../Bot.h"

namespace Sim {
	namespace Prog {
		void Sensor::execute(Bot* bot, BotCpu* cpu)
		{
			for(ProgramIdVec::iterator i=mResponseProg.begin();
				i!=mResponseProg.end(); i++) {
				Schedule &sch = *i;
				cpu->scheduleProgram(sch.mId, sch.mDelay);
			}
		}
		
		void Sensor::save(Save::BasePtr& fp)
		{
			fp.writeInt<uint32_t>(mResponseProg.size());
			for(ProgramIdVec::iterator i=mResponseProg.begin();
				i!=mResponseProg.end(); i++) {
				Schedule &sch = *i;
				fp.writeInt<uint32_t>(sch.mId);
				fp.writeInt<uint32_t>(sch.mDelay);
			}
		}
		
		void Sensor::load(Save::BasePtr& fp)
		{
			mResponseProg.clear();
			
			uint32_t count = fp.readInt<uint32_t>();
			for(uint32_t i=0; i<count; i++) {
				uint32_t id = fp.readInt<uint32_t>();
				uint32_t delay = fp.readInt<uint32_t>();
				
				mResponseProg.push_back( Schedule(id, delay) );
			}
		}
	}
}
