#include "Player.h"
#include "State.h"

namespace Sim {
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
	
	void PlayerData::addPlayer(Player& side)
	{
		side.sideId = mData.size();
		mData.push_back(side);
	}
	
	void PlayerData::copyState(State& state)
	{
		PlayerData &other = state.getPlayerData();
		
		mData = PlayerVec(other.mData);
	}
	
	void PlayerData::save(Save::BasePtr& fp)
	{
		fp.writeInt(mData.size());
		for(size_t i=0; i<mData.size(); i++) {
			Player &pl = mData[i];
			fp.writeInt(pl.sideId);
			fp.writeInt(pl.allyGroup);
		}
	}
	
}
