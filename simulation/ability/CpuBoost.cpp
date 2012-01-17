#include "CpuBoost.h"

#include "../Bot.h"
#include "../data/BotD.h"

namespace Sim {
	namespace Abil {
		CpuBoost::CpuBoost(Simulation* sim, IdType id, IdType typeId,
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
			fp << mData;
		}

		void CpuBoost::load(Save::BasePtr& fp)
		{
			fp >> mData;
		}

	}
}
