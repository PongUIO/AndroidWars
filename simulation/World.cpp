#include "World.h"
#include "State.h"
#include "Sync.h"

namespace Sim {
	
	World::World(Simulation *sim) :
		mOffScreen(0),
		mSim(sim)
	{}
	
	World::~World()
	{}
	
	void World::startup()
	{
		mWidth = mHeight = 32;
		
		mTileCol.startup(16.0, 4);
		
		mOffScreen = Tile(0);
		
		size_t totalSize = mWidth*mHeight;
		for(size_t i=0; i<totalSize; i++) {
			mData.push_back( mOffScreen );
		}
	}
	
	void World::shutdown()
	{
		mData.clear();
		
		mTileCol.shutdown();
	}
	
	void World::startPhase()
	{
		
	}
	
	void World::step(double stepTime)
	{
		
	}
	
	void World::endPhase()
	{
		
	}
	
	void World::checksum(Sync& sync)
	{
		for(size_t i=0; i<mWidth*mHeight; i++) {
			Tile &t = mData[i];
			sync.mixInt(t.mType);
			sync.mixInt(t.mColType.side);
			sync.mixInt(t.mColType.sl[0]);
			sync.mixInt(t.mColType.sl[1]);
		}
	}
	
	void World::copyState(State& state)
	{
		World &otherWorld = state.getWorld();
		for(size_t i=0; i<mWidth*mHeight; i++) {
			mData[i] = otherWorld.mData[i];
		}
	}
	
	Tile &World::getTile(uint32_t xInd, uint32_t yInd)
	{
		if(xInd >= mWidth || yInd >= mHeight)
			return mOffScreen;
		else
			return mData[yInd*mHeight + xInd];
	}
	
	
	Tile::Tile(uint16_t type) :
		mType(type),
		mColType(TileCol::TileType::Left,0,0)
	{}
	
	Tile::~Tile()
	{}
}