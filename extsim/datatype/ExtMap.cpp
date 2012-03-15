#include <boost/algorithm/string.hpp>

#include "ExtMap.h"

#include "../ExtData.h"
#include "../ExtSim.h"

namespace ExtS {
	// ExtMapData
	//
	//
	ExtMapData::ExtMapData(ExtSim& sim): ExtBaseData(sim)
	{}
	
	ExtMapData::~ExtMapData()
	{}
	
	void ExtMapData::startup()
	{}
	
	void ExtMapData::shutdown()
	{}
	
	void ExtMapData::loadBlock(Script::Block& block)
	{
		mExtSim.getSim().getConfiguration().tileSize =
			ExtData::readValue<double>(block.getDataFirst("TileSize"), 1.0);
		
		// Read tile types
		{
			Sim::TileDatabase &tileDb = mExtSim.getSim().getData().getTileDb();
			
			Script::Block *ttBlock = block.getBlock("TILE");
			if(ttBlock) {
				for(size_t i=0; i<ttBlock->getSize(); ++i) {
					Script::Data *ttData = ttBlock->getDataIndex(i);
					if(ttData) {
						Sim::TileD td;
						
						td.colMask = (Sim::TileD::ColMask)
							ExtData::readBitfield<uint32_t>(
							ttData->getArg(1), 0);
						
						tileDb.addTile(td);
					}
				}
			}
		}
		
		// Read dimensions
		uint32_t width, height;
		
		width = ExtData::readValue<uint32_t>(
			block.getDataFirst("Width"), 32);
		height = ExtData::readValue<uint32_t>(
			block.getDataFirst("Height"), 16);
		
		mExtSim.getSim().getState().getWorld().setDimensions(width,height);
		
		// Read tile data
		{
			std::string tileData = block.getDataFirst("TileData");
			typedef std::vector<std::string> StringVec;
			
			StringVec rowData;
			boost::split(rowData, tileData, boost::is_any_of("\n"),
				boost::algorithm::token_compress_on);
			for(uint32_t y=0; y<rowData.size(); ++y) {
				StringVec tile;
				boost::split(tile, rowData[y], boost::is_any_of("\t "),
					boost::algorithm::token_compress_on);
				
				for(uint32_t x=0; x<tile.size(); ++x) {
					uint32_t tileId =
						ExtData::readValue<uint32_t>(tile[x], 0);
					
					if(x<width && y<height) {
						mExtSim.getSim().getState().getWorld().getTile(x,y).
						setType(tileId);
					}
				}
			}
		}
	}
	
	void ExtMapData::postProcess()
	{}
}
