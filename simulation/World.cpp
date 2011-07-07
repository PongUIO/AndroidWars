#include <math.h>

#include "World.h"
#include "State.h"

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
		mTileSize = 1.0;
		
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
	
	void World::save(Save::BasePtr &fp)
	{
		fp.writeInt(mWidth);
		fp.writeInt(mHeight);
		for(size_t i=0; i<mWidth*mHeight; i++) {
			Tile &t = mData[i];
			fp.writeInt(t.mType);
			fp.writeInt(t.mColType.data);
		}
	}
	
	void World::copyState(State& state)
	{
		World &otherWorld = state.getWorld();
		for(size_t i=0; i<mWidth*mHeight; i++) {
			mData[i] = otherWorld.mData[i];
		}
	}
	
	World::ColResult World::collide(const Vector& pos, const Vector& vel, Collision* colObj)
	{
		static const int MaxIter = 4;
		
		World::ColResult tmpRet, res;
		tmpRet.colRes.isCol = false;
		tmpRet.colRes.n = Vector(0);
		tmpRet.colRes.dist = 0;
		
		int32_t top, left, right, bot;
		
		for(int i=0; i<MaxIter; i++) {
			// Get the adjusted coordinates of the collision object
			Vector lowPos =
				pos+tmpRet.colRes.getOrp() + colObj->getBboxLow();
			Vector highPos =
				pos+tmpRet.colRes.getOrp() + colObj->getBboxHigh();
			
			// Convert real coordinates to world indexes
			left  = getIndex(lowPos.x);
			top   = getIndex(lowPos.y);
			right = getIndex(highPos.x);
			bot   = getIndex(highPos.y);
			
			// [This space intentionally left blank]
			res.colRes.isCol = false;
			for(int32_t ix = left; ix<=right; ix++) {
				for(int32_t iy = top; iy<=bot; iy++) {
					const Tile &t = getTile(ix,iy);
					
					Collision *tileCol = mTileCol.getTileCol(t.getColType());
					
					Collision::Result res = colObj->check(
						pos+tmpRet.colRes.getOrp(), vel,
						Vector(getCoord(ix),getCoord(iy)), colObj
					);
					
					if(res.isCol) {
						tmpRet.colRes.isCol = true;
						tmpRet.colRes.n = res.n;
						tmpRet.colRes.dist = res.dist;
					}
				} // for(iy)
			} // for(ix)
			
			if(!res.colRes.isCol)
				break;
			
		} // for(iter)
		
		return tmpRet;
	} // collide()
	
	Tile &World::getTile(uint32_t xInd, uint32_t yInd)
	{
		if(xInd >= mWidth || yInd >= mHeight)
			return mOffScreen;
		else
			return mData[yInd*mHeight + xInd];
	}
	
	
	Tile::Tile(uint16_t type) :
		mType(type),
		mColType(TileCol::TileType::Left,0,0),
		mFlags(0)
	{}
	
	Tile::~Tile()
	{}
}
