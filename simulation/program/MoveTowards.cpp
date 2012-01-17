#include "MoveTowards.h"

#include "../Vector.h"
#include "../Bot.h"
#include "../bot/BotCpu.h"
#include "../Simulation.h"

namespace Sim {
	namespace Prog {
		MoveTowards::MoveTowards(Simulation *sim, IdType id, IdType typeId,
			const Config& cfg) :
			Program(sim, id, typeId), mType(cfg.mType),
			mTarget(cfg.mTargetId), mTargetPos(cfg.mTargetPos)
		{}
		
		MoveTowards::~MoveTowards()
		{}
		
		
		void MoveTowards::process(Bot* bot, BotCpu* cpu)
		{
			if(mTarget != NoId) {
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
			fp << mType << mTarget << mTargetPos;
			
			return Program::save(fp);
		}
		
		void MoveTowards::load(Save::BasePtr& fp)
		{
			fp >> mType >> mTarget >> mTargetPos;
			
			return Program::load(fp);
		}
	}
}
