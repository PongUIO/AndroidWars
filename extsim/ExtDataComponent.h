#ifndef EXTSIM_EXTDATACOMPONENT_H
#define EXTSIM_EXTDATACOMPONENT_H

#include "ExtDataListener.h"

namespace ExtS {
	class ExtDataComponent : public DataListener {
		public:
			virtual void startup()=0;
			virtual void shutdown()=0;
			
		private:
	};
}

#endif
