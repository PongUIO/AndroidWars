#include <boost/functional/hash.hpp>
#include <algorithm>

#include "Collision.h"
#include "TileCol.h"

//#include <stdio.h>
namespace Sim {
	bool operator==(TileCol::TileType const& a, TileCol::TileType const& b)
	{	return a.data == b.data;	}
	
	TileCol::TileCol()
	{}
	
	TileCol::~TileCol()
	{}
	
	void TileCol::startup(double tileSize, int slopeRes)
	{
		mTileSize = tileSize;
		
		mSlopeRes = std::min(slopeRes, 1<<TileType::SlopeBits);
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
			{ Vector(0,0), Vector(0,1) }
		},
		
		{ // Up
			{ 1, 2 },
			{ 3, 0 },
			Vector(0,-1),
			{ Vector(0,1), Vector(1,1) }
		},
		
		{ // Right
			{ 2, 3 },
			{ 0, 1 },
			Vector(-1,0),
			{ Vector(1,1), Vector(1,0) }
		},
		
		{ // Down
			{ 3, 0 },
			{ 1, 2 },
			Vector(0,1),
			{ Vector(1,0), Vector(0,0) }
		}
	};
	
	Collision* TileCol::generateTile(const Sim::TileCol::TileType& type)
	{
		Collision::ColPoints points(4);
		
		Dir &act = dirMap[type.getSide()];
		
		//printf("%d %d %d\n", type.getSide(), type.getSlopeL(), type.getSlopeR());
		//printf("%d %d\n", act.basePt[0], act.basePt[1]);
		points[act.basePt[0]] = act.off[0]*mTileSize;
		points[act.basePt[1]] = act.off[1]*mTileSize;
		points[act.modPt[0]] =
			act.off[1]*mTileSize + act.dir*getSlope(type.getSlopeL());
		points[act.modPt[1]] =
			act.off[0]*mTileSize + act.dir*getSlope(type.getSlopeR());
		//printf("%g %g, %g %g\n", points[0].x, points[0].y, points[1].x, points[1].y);
		//printf("%g %g, %g %g\n", points[2].x, points[2].y, points[3].x, points[3].y);
		
		return new Collision(points);
	}

}
