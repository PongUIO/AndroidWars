#include <boost/functional/hash.hpp>

#include "Collision.h"
#include "TileCol.h"

namespace Sim {
	bool operator==(TileCol::TileType const& a, TileCol::TileType const& b)
	{
		if(a.side != b.side)
			return false;
		
		for(int i=0; i<2; i++)
			if(a.sl[i] != b.sl[i])
				return false;
		
		return true;
	}

	std::size_t hash_value(const TileCol::TileType &val)
	{
		std::size_t seed = 0;
		
		boost::hash_combine(seed, val.side);
		for(int i=0; i<2; i++) {
			boost::hash_combine(seed, val.sl[i]);
		}
		
		return seed;
	}
	
	
	TileCol::TileCol()
	{}
	
	TileCol::~TileCol()
	{}
	
	void TileCol::startup(double tileSize, uint32_t slopeRes)
	{
		mTileSize = tileSize;
		mSlopeRes = slopeRes;
	}

	void TileCol::shutdown()
	{
		for(TileMap::iterator i=mData.begin(); i!=mData.end(); i++) {
			delete i->second;
		}
	}
	
	Collision* TileCol::getTileCol(const Sim::TileCol::TileType& type)
	{
		Collision *ret = NULL;
		TileMap::iterator i = mData.find(type);
		
		if(i == mData.end()) {
			ret = generateTile(type);
			mData[type] = ret;
		} else ret = i->second;
		
		return ret;
	}
	
	static struct Dir {
		uint32_t basePt[2];
		uint32_t modPt[2];
		Vector dir;
		Vector off[2];
	} dirMap[4] = {
		{ // Left
			{ 0, 1 },
			{ 2, 3 },
			Vector(1,0),
			{ Vector(0,1), Vector(0,0) }
		},
		
		{ // Up
			{ 1, 2 },
			{ 3, 0 },
			Vector(0,-1),
			{ Vector(1,1), Vector(0,1) }
		},
		
		{ // Right
			{ 2, 3 },
			{ 0, 1 },
			Vector(-1,0),
			{ Vector(1,0), Vector(1,1) }
		},
		
		{ // Down
			{ 3, 0 },
			{ 1, 2 },
			Vector(0,1),
			{ Vector(0,0), Vector(1,0) }
		}
	};
	
	Collision* TileCol::generateTile(const Sim::TileCol::TileType& type)
	{
		Collision::ColPoints points(4);
		
		Dir &act = dirMap[type.side];
		
		points[act.basePt[0]] = act.off[0]*mTileSize;
		points[act.basePt[1]] = act.off[1]*mTileSize;
		points[act.modPt[0]] =
			act.off[0]*mTileSize + act.dir*getSlope(type.sl[0]);
		points[act.modPt[1]] =
			act.off[1]*mTileSize + act.dir*getSlope(type.sl[1]);
		
		return new Collision(points);
	}

}
