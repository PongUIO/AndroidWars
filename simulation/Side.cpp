#include "Side.h"

namespace Sim {
	SideData::SideData()
	{}
	SideData::~SideData()
	{}
	
	void SideData::startup()
	{
		
	}

	void SideData::shutdown()
	{
		mData.clear();
	}
	
	void SideData::addSide(Side& side)
	{
		side.sideId = mData.size();
		mData.push_back(side);
	}
}
