#ifndef EXTSIM_EXTGAME_H
#define EXTSIM_EXTGAME_H

#include <string>

#include "../ExtDataComponent.h"

namespace ExtS {
	class ExtSim;
	
	class ExtGameData : public ExtDataComponent {
		public:
			ExtGameData(ExtSim& sim);
			virtual ~ExtGameData();
			
			void startup();
			void shutdown();
			
			void loadNode(const Nepeta::Node& node);
			void postProcess();
			
			const std::string &getName() const
			{ return mName; }
			const std::string &getDescription() const
			{ return mDescription; }
			
		private:
			ExtSim &mExtSim;
			
			std::string mName;
			std::string mDescription;
			
			double mStepTime;
	};
}

#endif
