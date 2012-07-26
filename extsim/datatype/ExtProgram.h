#ifndef EXTSIM_EXTPROGRAM_H
#define EXTSIM_EXTPROGRAM_H

#include "../DefaultExtData.h"
#include "../object/ExtDataObj.h"

namespace ExtS {
	class ExtSim;
	
	class ExtProgram : public ExtDataObjBase {
		public:
			ExtProgram(ExtSim &esim, Sim::IdType id);
			~ExtProgram();
			
			void loadNode(const Nepeta::Node& node);
			void postProcess();
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
