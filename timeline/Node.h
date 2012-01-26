#ifndef TM_NODE_H
#define TM_NODE_H

#include <vector>
#include <stdint.h>

#include "../simulation/Vector.h"

namespace Tm {
	class TimelineMgr;
	
	class Node;
	class NodeSequence;
	
	/**
	 * @brief Node on the timeline
	 * 
	 * Contains data related to what happens at a given position
	 * on the timline for a bot.
	 * 
	 * Nodes are normally chained in sequences, but any node
	 * may also branch off if due to special events.
	 */
	class Node {
		public:
			Node(TimelineMgr &mgr);
			~Node();
			
			Sim::Vector getOffsetPosition() const;
			
		private:
			TimelineMgr &mTimelineMgr;
			
			typedef std::vector<Node*> NodeVector;
			NodeVector mBranches;
			
			Sim::Vector mPos;
	};
}

#endif
