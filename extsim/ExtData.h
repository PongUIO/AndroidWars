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
			//@}
			
			/// @name Database retrieval
			//@{
				ArmorData &getArmorDb() { return mArmor; }
				DamageData &getDamageDb() { return mDamage; }
				BotData &getBotDb() { return mBot; }
				ProgramData &getProgramDb() { return mProgram; }
			//@}
			
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
			
		private:
			void registerListener(const std::string &blockTag, BaseData *db);
			BaseData *getListener(const std::string &tag);
			
			/// @name Databases
			//@{
				ArmorData mArmor;
				DamageData mDamage;
				BotData mBot;
				ProgramData mProgram;
			//@}
			
			/// @name Internal
			//@{
				typedef boost::unordered_map<std::string, BaseData*> ListenerMap;
				ListenerMap mListeners;
				
				ExtSim &mExtSim;
			//@}
	};
}

#endif
