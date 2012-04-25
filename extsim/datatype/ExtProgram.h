#ifndef EXTSIM_EXTPROGRAM_H
#define EXTSIM_EXTPROGRAM_H

#include "../ExtBaseData.h"
#include "../TypeRule.h"

namespace ExtS {
	class ExtSim;
	
	class ExtProgram : public ExtBaseDataObj {
		public:
			ExtProgram(ExtSim *esim);
			~ExtProgram();
			
			void loadBlock(Script::Block& block,
				Sim::IdType simTypeId, TypeRule* rule);
			void postProcess(ExtSim &extsim);
	};
	
	class ExtProgramData : public DefaultExtData<ExtProgram> {
		public:
			ExtProgramData(ExtSim &esim);
			virtual ~ExtProgramData();
			
			void startup() {}
			void shutdown() {}
	};
}

#endif
