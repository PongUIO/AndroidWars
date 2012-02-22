#include "Simulation.h"
#include "World.h"
#include "Data.h"
#include "data/TileD.h"
#include "State.h"

namespace Sim {
	
	World::World(Simulation *sim) :
		mWidth(0), mHeight(0), mTileSize(1.0),
		mOffScreen(0), mSim(sim)
	{}
	
	World::~World()
	{}
	
	void World::startup()
	{
		mTileSize = mSim->getConfiguration().tileSize;
		
		mOffScreen = Tile(0);
	}
	
	void World::shutdown()
	{
		mData.clear();
	}
	
	void World::setDimensions(uint32_t width, uint32_t height)
	{
		mData.clear();
		
		mWidth = width;
		mHeight = height;
		
		size_t totalSize = mWidth*mHeight;
		for(size_t i=0; i<totalSize; i++) {
			mData.push_back( mOffScreen );
		}
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
		fp << mWidth << mHeight;
		fp << mOffScreen;
		
		for(size_t i=0; i<mWidth*mHeight; i++) {
			fp << mData[i];
		}
	}
	
	void World::load(Save::BasePtr& fp)
	{
		mData.clear();
		
		fp >> mWidth >> mHeight;
		fp >> mOffScreen;
		
		mData.resize(mWidth*mHeight);
		for(size_t i=0; i<mWidth*mHeight; i++) {
			fp >> mData[i];
		}
	}
	
	World::ColResult World::collide(
		const Vector& pos, const Vector& vel,
		const Collision* colObj)
	{
		static const int MaxIter = 4;
		
		TileCol &tcolData = mSim->getData().getTileColDb();
		
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
					const TileD &tdata = mSim->getData().getTileDb().getTile(t.getType());
					
					Collision::Result cres;
					
					if(tdata.colMask & TileD::ColBot) {
						Collision *tileCol = tcolData.getTileCol(t.getColType());
						cres = colObj->check(
							pos+tmpRet.colRes.getOrp(), vel,
							Vector(getCoord(ix),getCoord(iy)), tileCol);
					}
					
					if(cres.isCol) {
						tmpRet.colRes.isCol = true;
						tmpRet.colRes.n = cres.n;
						tmpRet.colRes.dist = cres.dist;
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
