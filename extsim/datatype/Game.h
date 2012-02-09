#ifndef EXTSIM_GAME_H
#define EXTSIM_GAME_H

#include <string>

#include "../BaseData.h"

namespace ExtS {
	class GameData : public BaseData {
		public:
			GameData(ExtSim& sim);
			virtual ~GameData();
			
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
