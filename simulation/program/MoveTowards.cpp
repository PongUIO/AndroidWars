#include "MoveTowards.h"

#include "../Vector.h"
#include "../Bot.h"
#include "../bot/BotCpu.h"
#include "../Simulation.h"

namespace Sim {
	namespace Prog {
		MoveTowards::MoveTowards(Simulation *sim, uint32_t id, uint32_t typeId,
			const Config& cfg) :
			Program(sim, id, typeId), mType(cfg.mType),
			mTarget(cfg.mTargetId), mTargetPos(cfg.mTargetPos)
		{}
		
		MoveTowards::~MoveTowards()
		{}
		
		
		void MoveTowards::process(Bot* bot, BotCpu* cpu)
		{
			if(mTarget != FactoryNoId) {
				Bot *target = mSim->getState().getBotFactory().getBot(mTarget);
				if(target)
					mTargetPos = target->getState().mBody.mPos;
			}
			
			Bot::State &bState = bot->getState();
			if(mTargetPos != bState.mBody.mPos) {
				Vector dir = (mTargetPos - bState.mBody.mPos).normalize();
				bState.mEngine.mDirection = dir;
				bState.mEngine.mStrength = 10.0;
			}
		}
		
		void MoveTowards::save(Save::BasePtr& fp)
		{
			fp.writeInt<uint8_t>(mType);
			fp.writeInt<uint32_t>(mTarget);
			fp.writeVec(mTargetPos);
			
			return Program::save(fp);
		}
		
		void MoveTowards::load(Save::BasePtr& fp)
		{
			mType = (DestinationType)fp.readInt<uint8_t>();
			mTarget = fp.readInt<uint32_t>();
			mTargetPos = fp.readVec();
			
			return Program::load(fp);
		}
	}
}
