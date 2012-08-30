#ifndef EXTSIM_EXTMODULE_H
#define EXTSIM_EXTMODULE_H

#include "../simulation/Save.h"

namespace exts {
	class ExtModule {
		public:
			ExtModule() {}
			virtual ~ExtModule() {}
			
			virtual void startup()=0;
			virtual void shutdown()=0;
			
			virtual void save(Sim::Save::BasePtr &fp) const { (void)fp; }
			virtual void load(Sim::Save::BasePtr &fp) { (void)fp; }
	};
}

#endif
