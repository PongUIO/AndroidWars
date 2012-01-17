#include "Player.h"
#include "State.h"

namespace Sim {
	// Player
	//
	//
	
	// PlayerData
	//
	//
	PlayerData::PlayerData()
	{}
	PlayerData::~PlayerData()
	{}
	
	void PlayerData::startup()
	{
		
	}

	void PlayerData::shutdown()
	{
		mData.clear();
	}
	
	void PlayerData::step(double stepTime)
	{
		
	}
	
	void PlayerData::addPlayer(Player& side)
	{
		side.mSideId = mData.size();
		mData.push_back(side);
	}
	
	void PlayerData::save(Save::BasePtr& fp)
	{
		fp.writeCtr(mData);
	}
	
	void PlayerData::load(Save::BasePtr& fp)
	{
		fp.readCtr(mData);
	}
}
