#ifndef EXTSIM_PROGRAM_H
#define EXTSIM_PROGRAM_H

#include "../BaseData.h"
#include "../TypeRule.h"

namespace ExtS {
	class ExtSim;
	
	class ExtProgram {
		public:
			ExtProgram();
			~ExtProgram();
			
			void loadBlock(Script::Block &block, TypeRule *rule);
			void postProcess(ExtSim &extsim);
			
			const std::string &getName() const { return mName; }
			const std::string &getDescription() const { return mDescription; }
			
		private:
			Sim::IdType mId;
			
			std::string mName;
			std::string mDescription;
			
			TypeRule *mRule;
			friend class Sim::DataCtr<ExtProgram>;
	};
	
	class ProgramData : public DefaultData<ExtProgram>, public TypeRuleMgr {
		public:
			ProgramData(ExtSim &esim);
			virtual ~ProgramData();
			
			void startup() {}
			void shutdown() {}
			
			void loadBlock(Script::Block& block);
			void postProcess();
			
		private:
	};
}

#endif
