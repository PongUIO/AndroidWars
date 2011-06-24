#include "Data.h"

namespace Sim {
	Data::Data()
	{}
	
	Data::~Data()
	{}
	
	void Data::startup()
	{
		mTile.startup();
	}
	
	void Data::shutdown()
	{
		mTile.shutdown();
	}
	
	void Data::checksum(Save::SyncPtr &sync)
	{
		mTile.checksum(sync);
	}
}
