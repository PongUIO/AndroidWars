#ifndef SIM_PLAYER_H
#define SIM_PLAYER_H

#include <stdint.h>
#include <vector>

#include "StateObj.h"

#include "Save.h"
#include "bot/BotAbility.h"

namespace Sim {
	class State;
	
	struct Player : private Save::OperatorImpl<Player> {
		uint32_t mSideId;
		uint32_t mAllyGroup;
		
		BotAbility::AvailableProgram mBasePrograms; /// @todo Save bitsets
		BotAbility::AbilityList mGlobalAbilities;
		
		void save(Save::BasePtr &fp) const
		{	fp << mSideId << mAllyGroup;
			fp.writeCtr(mGlobalAbilities); }
		void load(Save::BasePtr &fp)
		{	fp >> mSideId >> mAllyGroup;
			fp.readCtr(mGlobalAbilities); }
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
				Player &getPlayer(IdType id)
				{ return mData[id]; }
			//@}
			
		private:
			typedef std::vector<Player> PlayerVec;
			
			PlayerVec mData;
	};
	
}

#endif
