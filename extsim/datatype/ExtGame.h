#ifndef EXTSIM_EXTGAME_H
#define EXTSIM_EXTGAME_H

#include <string>

#include "../ExtBaseData.h"

namespace ExtS {
	class ExtGameData : public ExtBaseData {
		public:
			ExtGameData(ExtSim& sim);
			virtual ~ExtGameData();
			
			void startup();
			void shutdown();
			
			void loadBlock(Script::Block& block);
			
			void postProcess();
			
			const std::string &getName() const
			{ return mName; }
			const std::string &getDescription() const
			{ return mDescription; }
			
		private:
			std::string mName;
			std::string mDescription;
			
			double mStepTime;
	};
}

#endif
