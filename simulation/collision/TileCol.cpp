#include <boost/functional/hash.hpp>

#include "Collision.h"
#include "TileCol.h"

namespace Sim {
	bool operator==(TileCol::TileType const& a, TileCol::TileType const& b)
	{
		for(int i=0; i<TileCol::TileType::MaxSide; i++) {
			if(a.slope[i] != b.slope[i])
				return false;
		}
		return true;
	}

	std::size_t hash_value(const TileCol::TileType &val)
	{
		std::size_t seed = 0;
		
		for(int i=0; i<TileCol::TileType::MaxSide; i++) {
			boost::hash_combine(seed, val.slope[i]);
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
	
	Collision* TileCol::generateTile(const Sim::TileCol::TileType& type)
	{
		Collision::ColPoints points(4);
		
		points[TileType::Left] =
			Vector(0, 0+getSlope(type.slope[TileType::Left]) );
		points[TileType::Up] =
			Vector(0+getSlope(type.slope[TileType::Up]), 0 );
		points[TileType::Right] =
			Vector(mTileSize, getSlope(type.slope[TileType::Right]) );
		points[TileType::Down] =
			Vector(mTileSize, mTileSize-getSlope(type.slope[TileType::Down]) );
		
		return new Collision(points);
	}

}
