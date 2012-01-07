#ifndef SIM_PLAYER_H
#define SIM_PLAYER_H

#include <stdint.h>
#include <vector>

#include "StateObj.h"

#include "Save.h"
#include "bot/BotAbility.h"

namespace Sim {
	class State;
	
	struct Player {
		uint32_t sideId;
		uint32_t allyGroup;
		
		BotAbility::AvailableProgram mBasePrograms;
		BotAbility::AbilityList mGlobalAbilities;
	};
	
	class PlayerData : public StateObj {
		public:
			/// @name Initialization
			//@{
				PlayerData();
				~PlayerData();
			//@}
			
			/// @name StateObj calls
			//@{
				void startup();
				void shutdown();
				
				void step(double stepTime);
				
				void save(Save::BasePtr &fp);
				void load(Save::BasePtr& fp);
			//@}
			
			/// @name Interface
			//@{
				void addPlayer(Player &player);
				Player &getPlayer(uint32_t id)
				{ return mData[id]; }
			//@}
			
		private:
			typedef std::vector<Player> PlayerVec;
			
			PlayerVec mData;
	};
	
}

#endif
