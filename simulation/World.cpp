#include "World.h"

namespace Sim {
	
	World::World(Simulation *sim) :
		mSim(sim)
	{}
	
	World::~World()
	{}
	
	void World::startup()
	{
		mWidth = mHeight = 32;
		
		mTileCol.startup(16.0, 4);
		
		size_t totalSize = mWidth*mHeight;
		for(size_t i=0; i<totalSize; i++) {
			mData.push_back( new Tile() );
		}
		
		mOffScreen = new Tile();
	}
	
	void World::shutdown()
	{
		for(size_t i=0; i<mData.size(); i++) {
			delete mData[i];
		}
		
		delete mOffScreen;
		
		mTileCol.shutdown();
	}
	
	Tile *World::getTile(uint32_t xInd, uint32_t yInd)
	{
		if(xInd >= mWidth || yInd >= mHeight)
			return mOffScreen;
		else
			return mData[yInd*mHeight + xInd];
	}
}
