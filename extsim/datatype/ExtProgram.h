#ifndef EXTSIM_EXTPROGRAM_H
#define EXTSIM_EXTPROGRAM_H

#include "../DefaultExtData.h"
#include "../TypeRuleLoader.h"
#include "../object/ExtDataObj.h"

namespace exts {
	class ExtSim;
	
	class ExtProgram : public ExtDataObjBase {
		public:
			ExtProgram(ExtSim &esim, Sim::IdType id);
			~ExtProgram();
			
			void loadNode(const Nepeta::Node& node);
			void postProcess();
			
			const TypeRule *getRule() const { return mRule; }
			
		private:
			TypeRule *mRule;
	};
	
	class ExtProgramData : public DefaultExtData<ExtProgram> {
		public:
			ExtProgramData(ExtSim &esim);
			virtual ~ExtProgramData();
			
			void startup() {}
			void shutdown() {}
			
		private:
			
			TypeRuleLoader mRuleLoader;
			friend class ExtProgram;
	};
}

#endif
