#ifndef EXTSIM_EXTPROGRAM_H
#define EXTSIM_EXTPROGRAM_H

#include "../ExtBaseData.h"
#include "../TypeRule.h"

namespace ExtS {
	class ExtSim;
	
	class ExtProgram : public ExtBaseDataObj {
		public:
			ExtProgram();
			~ExtProgram();
			
			void loadBlock(Script::Block& block, TypeRule* rule);
			void postProcess(ExtSim &extsim);
			
			/*const std::string &getName() const { return mName; }
			const std::string &getDescription() const { return mDescription; }
			
			const TypeRule *getRule() const { return mRule; }
			Sim::IdType getId() const { return mId; }*/
			
		private:
			/*Sim::IdType mId;
			
			std::string mName;
			std::string mDescription;
			
			TypeRule *mRule;
			friend class Sim::DataCtr<ExtProgram>;*/
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
