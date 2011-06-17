#include "World.h"

namespace Sim {
	
	World::World(uint32_t w, uint32_t h) :
		mWidth(w), mHeight(h)
	{
		size_t totalSize = w*h;
		for(size_t i=0; i<totalSize; i++) {
			mData.push_back( new Tile() );
		}
		
		mOffScreen = new Tile();
	}
	
	World::~World()
	{
		delete[] mData;
	}
	
	Tile *World::getTile(uint32_t xInd, uint32_t yInd)
	{
		if(xInd >= mWidth || yInd >= mHeight)
			return mOffScreen;
		else
			return mData[yInd*mHeight + xInd];
	}
}
