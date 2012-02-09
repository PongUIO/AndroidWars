#ifndef EXTSIM_EXTPLAYER_H
#define EXTSIM_EXTPLAYER_H

#include <stdint.h>

namespace ExtS {
	class ExtPlayer {
		public:
			ExtPlayer();
			~ExtPlayer();
			
		private:
			/**
			 * Constraint limits
			 */
			struct Limits {
				uint32_t mCost;
				uint32_t mBotCount;
			} mLimits;
	};
	
	class ExtPlayerManager {
		public:
			
		private:
	};
}

#endif
