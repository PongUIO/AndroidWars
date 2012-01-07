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
	
	void PlayerData::step(double stepTime)
	{
		
	}
	
	void PlayerData::addPlayer(Player& side)
	{
		side.sideId = mData.size();
		mData.push_back(side);
	}
	
	void PlayerData::save(Save::BasePtr& fp)
	{
		fp.writeInt<uint32_t>(mData.size());
		for(size_t i=0; i<mData.size(); i++) {
			Player &pl = mData[i];
			fp.writeInt<uint32_t>(pl.sideId);
			fp.writeInt<uint32_t>(pl.allyGroup);
		}
	}
	
	void PlayerData::load(Save::BasePtr& fp)
	{
		mData.clear();
		
		uint32_t count = fp.readInt<uint32_t>();
		for(uint32_t i=0; i<count; i++) {
			Player pl;
			pl.sideId = fp.readInt<uint32_t>();
			pl.allyGroup = fp.readInt<uint32_t>();
		}
	}
}
