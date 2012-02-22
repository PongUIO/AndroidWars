#ifndef SIM_DATA_H
#define SIM_DATA_H

#include "CommonTemplate.h"

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
	
#define _SIM_X_DATA_COMPONENTS \
	_SIM_X(TileDatabase, Tile) \
	_SIM_X(BotDatabase, Bot) \
	_SIM_X(BulletDatabase, Bullet) \
	_SIM_X(WeaponDatabase, Weapon) \
	_SIM_X(ProgramDatabase, Program) \
	_SIM_X(AbilityDatabase, Ability) \
	_SIM_X(ArmorDatabase, Armor) \
	_SIM_X(DamageDatabase, Damage) \
	_SIM_X(TileCol, TileCol)
	
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
				template<class T>
				T &getDatabase();
				
				template<class T>
				typename T::DatabaseType &getDatabaseByRelated()
				{ return getDatabase<typename T::DatabaseType>(); }
				
#define _SIM_X(type, name) type &get##name##Db() { return m##name; }
				_SIM_X_DATA_COMPONENTS
#undef _SIM_X
			//@}
			
		private:
#define _SIM_X(type, name) type m##name;
				_SIM_X_DATA_COMPONENTS
#undef _SIM_X
			
			Simulation *mSim;
	};
}

#endif
