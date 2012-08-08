#ifndef EXTSIM_EXTDATA_H
#define EXTSIM_EXTDATA_H

#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>

#include "nepeta.h"

#include "CommonTemplate.h"

#include "datatype/ExtAbility.h"
#include "datatype/ExtArmor.h"
#include "datatype/ExtBot.h"
#include "datatype/ExtDamage.h"
#include "datatype/ExtProgram.h"
#include "datatype/ExtWeapon.h"
#include "datatype/ExtGame.h"
#include "datatype/ExtMap.h"

namespace exts {
#define _EXTS_X_EXTDATA_COMPONENTS \
	_EXTS_X(ExtArmorData, Armor) \
	_EXTS_X(ExtDamageData, Damage) \
	_EXTS_X(ExtBotData, Bot) \
	_EXTS_X(ExtProgramData, Program) \
	_EXTS_X(ExtWeaponData, Weapon) \
	_EXTS_X(ExtGameData, Game) \
	_EXTS_X(ExtMapData, Map)
	
	/**
	 * @brief Data manager for the extended simulation.
	 * 
	 * Contains the databases for data related to the extended simulation,
	 * as well as a system to load these databases through script strings.
	 * 
	 * Scripts are assumed to be in the Nepeta language.
	 */
	class ExtData {
		public:
			/// @name Initialization
			//@{
				ExtData(ExtSim &esim);
				~ExtData();
				
				void startup();
				void shutdown();
			//@}
			
			/// @name Interface
			//@{
				void loadScript(const std::string &data);
				
				void postProcess();
				
				void switchContext(ListenerGroup *ctx);
				void registerListener(const std::string &blockTag, DataListener *entry);
			//@}
			
			/// @name Database retrieval
			//@{
				template<class T>
				T &getComponent();
				
				// Definition of getters for ExtData databases
				// Type &getTypeDb() { return mName; }
#define _EXTS_X(type, name) type &get##name##Db() { return m##name; }
				_EXTS_X_EXTDATA_COMPONENTS
#undef _EXTS_X
			//@}
			
		private:
			/// @name Databases
			//@{
				// Definition of ExtData databases
				// Type mName;
#define _EXTS_X(type, name) type m##name;
				_EXTS_X_EXTDATA_COMPONENTS
#undef _EXTS_X
			//@}
			
			/// @name Internal
			//@{
				ExtSim &mExtSim;
				
				ListenerGroup *mDefaultListener;
				ListenerGroup *mActiveListener;
			//@}
	};
}

#endif
