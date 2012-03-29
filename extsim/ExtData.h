#ifndef EXTSIM_EXTDATA_H
#define EXTSIM_EXTDATA_H

#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>

#include "CommonTemplate.h"
#include "ExtBaseData.h"

#include "datatype/ExtAbility.h"
#include "datatype/ExtArmor.h"
#include "datatype/ExtBot.h"
#include "datatype/ExtDamage.h"
#include "datatype/ExtProgram.h"
#include "datatype/ExtWeapon.h"
#include "datatype/ExtGame.h"
#include "datatype/ExtMap.h"

namespace ExtS {
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
	 * Scripts are assumed to be in the DaScript language.
	 */
	class ExtData {
		public:
			enum ListenerContext {
				LcNone					= 0x00,
				LcDataLoading			= 0x01,
				LcContentLoading		= 0x02
			};
			
			struct Listener {
				Listener(ExtBaseData *data=0, ListenerContext context=LcNone) :
					mData(data), mContext(context) {}
				
				ExtBaseData *mData;
				ListenerContext mContext;
			};
			
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
				void switchContext(ListenerContext ctx)
				{ mCurrentContext = ctx; }
			//@}
			
			/// @name Database retrieval
			//@{
				template<class T>
				T &getComponent();
				
#define _EXTS_X(type, name) type &get##name##Db() { return m##name; }
				_EXTS_X_EXTDATA_COMPONENTS
#undef _EXTS_X
			//@}
			
		private:
			void registerListener(const std::string &blockTag,
				const Listener &listener);
			ExtBaseData *getListener(const std::string &tag);
			
			/// @name Databases
			//@{
#define _EXTS_X(type, name) type m##name;
				_EXTS_X_EXTDATA_COMPONENTS
#undef _EXTS_X
			//@}
			
			/// @name Internal
			//@{
				typedef boost::unordered_map<std::string, Listener> ListenerMap;
				ListenerMap mListeners;
				ListenerContext mCurrentContext;
				
				ExtSim &mExtSim;
			//@}
			
			
		public:
			template<class T>
			static T badCastStrategy(const std::string &str, T def) { return def; }
			
			template<class T>
			static T readValue(const std::string &str, T def=T())
			{
				T val = def;
				if(str.empty())
					return def;
				
				try { val = boost::lexical_cast<T>(str); }
				catch(boost::bad_lexical_cast &)
				{ val=badCastStrategy<T>(str,def); }
				
				return val;
			}
			
			template<class T>
			static T readBitfield(const std::string &str,
				T def=T()) {
				T val = def;
				if(str.empty())
					return def;
				
				size_t bitIndex = 0;
				for(size_t i=0; i<str.size(); ++i) {
					if(str[i] == '1')
						val |= (1<<bitIndex);
				}
				
				return val;
			}
	};
	
	template<>
	inline Sim::Vector ExtData::readValue<Sim::Vector>(const std::string &str,
		Sim::Vector def) {
		std::vector<std::string> sep;
		boost::split(sep, str, boost::is_any_of(" ,"),
			boost::algorithm::token_compress_on);
		
		Sim::Vector val = def;
		if(sep.size() == 2) {
			val.x = readValue<double>(sep[0]);
			val.y = readValue<double>(sep[1]);
		}
		
		return val;
	}
}

#endif
