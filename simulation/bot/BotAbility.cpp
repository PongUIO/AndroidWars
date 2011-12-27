#include "BotAbility.h"
#include "../Bot.h"
#include "../Player.h"
#include "../data/BotD.h"

namespace Sim {
	BotAbility::BotAbility()
	{}
	
	BotAbility::~BotAbility()
	{}
	
	void BotAbility::initialize(Bot* host)
	{
		mHost = host;
	}
	
	void BotAbility::prepareStep(double delta)
	{
		mAvailProgram.clear();
		
		mAvailProgram.mergeWith(mHost->getPlayerPtr()->mBasePrograms);
		mAvailProgram.mergeWith(mHost->getTypePtr()->baseProgram);
		mAvailProgram.excludeSet(mHost->getTypePtr()->excludeProgram);
	}
	
	void BotAbility::updateCpu(double delta)
	{
		
	}
	
	void BotAbility::step(double delta)
	{
		
	}
}
