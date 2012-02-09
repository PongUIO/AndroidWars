#ifndef EXTSIM_EXTDATA_H
#define EXTSIM_EXTDATA_H

#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>

#include "BaseData.h"

#include "datatype/Ability.h"
#include "datatype/Armor.h"
#include "datatype/Bot.h"
#include "datatype/Damage.h"
#include "datatype/Program.h"
#include "datatype/Weapon.h"
#include "datatype/Game.h"
#include "datatype/Map.h"

namespace ExtS {
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
				Listener(BaseData *data=0, ListenerContext context=LcNone) :
					mData(data), mContext(context) {}
				
				BaseData *mData;
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
				ArmorData &getArmorDb() { return mArmor; }
				DamageData &getDamageDb() { return mDamage; }
				BotData &getBotDb() { return mBot; }
				ProgramData &getProgramDb() { return mProgram; }
				GameData &getGameDb() { return mGame; }
				MapData &getMapDb() { return mMap; }
			//@}
			
		private:
			void registerListener(const std::string &blockTag,
				const Listener &listener);
			BaseData *getListener(const std::string &tag);
			
			/// @name Databases
			//@{
				ArmorData mArmor;
				DamageData mDamage;
				BotData mBot;
				ProgramData mProgram;
				GameData mGame;
				MapData mMap;
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
			
			static Sim::Vector readVector(const std::string &str,
				Sim::Vector def=Sim::Vector()) {
				typedef std::vector<std::string> StringVec;
				StringVec sep;
				boost::algorithm::split(sep, str, boost::is_any_of(" ,"),
					boost::algorithm::token_compress_on);
				
				Sim::Vector vec = def;
				if(sep.size() == 2) {
					vec.x = readValue<double>(sep[0]);
					vec.y = readValue<double>(sep[1]);
				}
				
				return vec;
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
}

#endif
