#ifndef SIM_DATA_H
#define SIM_DATA_H

#include "Save.h"
#include "data/TileD.h"
#include "data/BotD.h"
#include "data/BulletD.h"
#include "data/WeaponD.h"
#include "data/ProgramD.h"
#include "data/AbilityD.h"
#include "data/ArmorD.h"
#include "data/DamageD.h"
#include "collision/TileCol.h"

#include "data/BaseData.h"
#include "utility/CallGroup.h"

namespace Sim {
	class Simulation;
	
	class Data : private CallGroup<BaseData>  {
		public:
			Data(Simulation *sim);
			~Data();
			
			void startup();
			void shutdown();
			
			void save(Save::BasePtr &sync);
			
			/// @name Database retrieval
			//@{
				// Databases
				TileDatabase &getTileDb() { return mTile; }
				BotDatabase &getBotDb() { return mBot; }
				BulletDatabase &getBulletDb() { return mBullet; }
				WeaponDatabase &getWeaponDb() { return mWeapon; }
				ProgramDatabase &getProgramDb()  { return mProgram; }
				AbilityDatabase &getAbilityDb() { return mAbility; }
				ArmorDatabase &getArmorDb() { return mArmor; }
				DamageDatabase &getDamageDb() { return mDamage; }
				
				// Other constant data
				TileCol &getTileCol() { return mTileCol; }
			//@}
			
		private:
			TileDatabase mTile;
			BotDatabase mBot;
			BulletDatabase mBullet;
			WeaponDatabase mWeapon;
			ProgramDatabase mProgram;
			AbilityDatabase mAbility;
			ArmorDatabase mArmor;
			DamageDatabase mDamage;
			
			TileCol mTileCol;
			
			Simulation *mSim;
	};
}

#endif
