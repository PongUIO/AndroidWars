#ifndef SIM_PLAYER_H
#define SIM_PLAYER_H

#include <stdint.h>
#include <vector>

#include "Save.h"

namespace Sim {
	class State;
	
	struct Player {
		uint32_t sideId;
		uint32_t allyGroup;
		
	};
	
	class PlayerData {
		public:
			/// @name Initialization
			//@{
				PlayerData();
				~PlayerData();
				
				void startup();
				void shutdown();
			//@}
			
			/// @name Interface
			//@{
				void addPlayer(Player &player);
				const Player &getPlayer(uint32_t id) const
				{ return mData[id]; }
			//@}
			
			/// @name State
			//@{
				void copyState(State &state);
				
				void save(Save::BasePtr &fp);
			//@}
			
		private:
			typedef std::vector<Player> PlayerVec;
			
			PlayerVec mData;
	};
	
}

#endif
