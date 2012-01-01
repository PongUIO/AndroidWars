#include "CpuBoost.h"

#include "../Bot.h"
#include "../data/BotD.h"

namespace Sim {
	namespace Abil {
		CpuBoost::CpuBoost(Simulation* sim, uint32_t id, uint32_t typeId,
			const Config& cfg): Ability(sim, id, typeId), mData(cfg)
		{}
		
		CpuBoost::~CpuBoost()
		{}
		
		void CpuBoost::prepareStep(double delta, Bot *bot)
		{
			const BotD *data = bot->getTypePtr();
			bot->getState().mCpu.feedCycles(
				mData.mAbsolute + data->cpuCycleSpeed*mData.mPercentage);
		}
		
		void CpuBoost::updateCpu(double delta, Bot *bot)
		{}
		
		void CpuBoost::step(double delta, Bot *bot)
		{}
		
		void CpuBoost::save(Save::BasePtr& fp)
		{
			fp.writeFloat(mData.mPercentage);
			fp.writeInt<int32_t>(mData.mAbsolute);
		}

		void CpuBoost::load(Save::BasePtr& fp)
		{
			mData.mPercentage = fp.readFloat();
			mData.mAbsolute = fp.readInt<int32_t>();
		}

	}
}
