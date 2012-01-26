#ifndef EXTSIM_PROGRAM_H
#define EXTSIM_PROGRAM_H

#include "../BaseData.h"
#include "../TypeRule.h"

namespace ExtS {
	class ExtProgram {
	};
	
	class ProgramData : public DefaultData<ExtProgram>, public TypeRuleMgr {
		public:
			ProgramData(Sim::Simulation& sim);
			virtual ~ProgramData();
			
			void startup();
			void shutdown() {}
			
			void loadBlock(Script::Block& block);
			void postProcess();
			
		private:
	};
}

#endif
