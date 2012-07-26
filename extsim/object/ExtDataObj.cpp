#include "ExtDataObj.h"
#include "../CommonLoad.h"

namespace ExtS {
	// ExtDataObjBase
	// 
	// 
	void ExtDataObjBase::loadNode(const Nepeta::Node& node)
	{
		node.readChain()
			.nodearg(Load::Name, mName)
			.nodearg(Load::Description, mDescription);
	}
	
	void ExtDataObjBase::postProcess()
	{}
}
