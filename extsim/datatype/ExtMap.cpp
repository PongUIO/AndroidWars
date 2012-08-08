#include <boost/algorithm/string.hpp>

#include "ExtMap.h"

#include "../ExtData.h"
#include "../util/TypeConv.h"
#include "../ExtSim.h"

namespace exts {
	// ExtMapData
	//
	//
	ExtMapData::ExtMapData(ExtSim& esim): ExtDataComponent(), mExtSim(esim)
	{}
	
	ExtMapData::~ExtMapData()
	{}
	
	void ExtMapData::startup()
	{}
	
	void ExtMapData::shutdown()
	{}
	
	void ExtMapData::loadNode(const Nepeta::Node& node)
	{
		uint32_t width, height;
		
		width = convValue(node.getNodeFirst("Width"), 1);
		height = convValue(node.getNodeFirst("Height"), 1);
		mExtSim.getSim().getConfiguration().tileSize =
			convValue(node.getNodeFirst("TileSize") ,1.0);
		
		// Read tile types
		{
			Sim::TileDatabase &tileDb = mExtSim.getSim().getData().getTileDb();
			
			const Nepeta::Node &ttNode = node.getNode("TILE");
			for(Nepeta::Iterator i=node.begin("TILE"); node.hasNext(i);
			node.next(i,"TILE")) {
				const Nepeta::Node &ttData = ttNode.getIterNode(i);
				Sim::TileD td;
				td.colMask = (Sim::TileD::ColMask)
					convBitfield<uint32_t>(
					ttData.getArg(1), 0);
				
				tileDb.addTile(td);
			}
		}
		
		// Set dimensions
		mExtSim.getSim().getState().getWorld().setDimensions(width,height);
		
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
						convValue<uint32_t>(tile[x], 0);
					
					if(x<width && y<height) {
						mExtSim.getSim().getState().getWorld().
						getTile(x,height-1-y).setType(tileId);
					}
				}
			}
		}
	}
	
	void ExtMapData::postProcess()
	{}
}
