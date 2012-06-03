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
	
	void ExtMapData::loadNode(Nepeta::Node& node)
	{
		uint32_t width, height;
		
		node.readChain().
			nodearg("TileSize", mExtSim->getSim().getConfiguration().tileSize).
			nodearg("Width", width).
			nodearg("Height", height);
		
		// Read tile types
		{
			Sim::TileDatabase &tileDb = mExtSim->getSim().getData().getTileDb();
			
			Nepeta::Node &ttNode = node.getNode("TILE");
			for(size_t i=0, nc=ttNode.getNodeCount(); i<nc; ++i) {
				Nepeta::Node &ttData = ttNode.getNode(i);
				Sim::TileD td;
				
				td.colMask = (Sim::TileD::ColMask)
					ExtData::readBitfield<uint32_t>(
					ttData.getArg(1), 0);
				
				tileDb.addTile(td);
			}
		}
		
		// Set dimensions
		mExtSim->getSim().getState().getWorld().setDimensions(width,height);
		
		// Read tile data
		{
			std::string tileData = node.getNodeFirst("TileData");
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
						mExtSim->getSim().getState().getWorld().getTile(x,height-1-y).
						setType(tileId);
					}
				}
			}
		}
	}
	
	void ExtMapData::postProcess()
	{}
}
