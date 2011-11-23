#ifndef SIM_PLAYER_H
#define SIM_PLAYER_H

#include <stdint.h>
#include <vector>

#include "StateObj.h"

#include "Save.h"

namespace Sim {
	class State;
	
	struct Player {
		uint32_t sideId;
		uint32_t allyGroup;
		
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
			//@}
			
			/// @name Interface
			//@{
				void addPlayer(Player &player);
				const Player &getPlayer(uint32_t id) const
				{ return mData[id]; }
			//@}
			
		private:
			typedef std::vector<Player> PlayerVec;
			
			PlayerVec mData;
	};
	
}

#endif
