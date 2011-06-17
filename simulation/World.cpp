#include "World.h"

namespace Sim {
	
	World::World(Simulation *sim, uint32_t w, uint32_t h) :
		mWidth(w), mHeight(h), mSim(sim)
	{
		size_t totalSize = w*h;
		for(size_t i=0; i<totalSize; i++) {
			mData.push_back( new Tile() );
		}
		
		mOffScreen = new Tile();
	}
	
	World::~World()
	{
		for(size_t i=0; i<mData.size(); i++) {
			delete mData[i];
		}
		
		delete mOffScreen;
	}
	
	Tile *World::getTile(uint32_t xInd, uint32_t yInd)
	{
		if(xInd >= mWidth || yInd >= mHeight)
			return mOffScreen;
		else
			return mData[yInd*mHeight + xInd];
	}
}
