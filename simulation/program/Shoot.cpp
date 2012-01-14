#include "Shoot.h"

#include "../Bot.h"
#include "../bot/BotWeapon.h"

namespace Sim { namespace Prog {
	Shoot::Shoot(Simulation* sim, uint32_t id, uint32_t typeId,
		const Config& cfg) :
		Program(sim, id, typeId),
		mTargetIndex(cfg.mTargetIndex),
		mWeapArg(cfg.mWeapArg)
	{}
	
	Shoot::~Shoot()
	{}
	
	uint32_t Shoot::getCycleCost()
	{	return 1; }
	
	void Shoot::process(Bot* bot, BotCpu* cpu)
	{
		bot->getState().mWeapon.addOrder(mTargetIndex, mWeapArg);
	}
	
	void Shoot::save(Save::BasePtr& fp)
	{
		fp.writeInt<uint32_t>(mTargetIndex);
		fp.writeSave(mWeapArg);
		
		return Program::save(fp);
	}

	void Shoot::load(Save::BasePtr& fp)
	{
		mTargetIndex = fp.readInt<uint32_t>();
		mWeapArg = fp.readSave();
		
		return Program::load(fp);
	}
} }
