#ifndef SIM_BOTABILITY_H
#define SIM_BOTABILITY_H

#include <boost/dynamic_bitset.hpp>
#include <boost/bind.hpp>
#include <boost/unordered_set.hpp>

#include "../Program.h"

namespace Sim {
	class Bot;
	class Ability;
	
	/**
	 * Manages both passive and active abilities for bots.
	 * 
	 * Active abilities are managed by a bitfield that enables or disables
	 * programs with a specific type ID. Available programs are merged from
	 * bot data and the bot owner's data.
	 * 
	 * Passive abilities are run in a similar manner to programs, except
	 * there is no CPU cost for running them (and they're forced to be
	 * executed every step). Removal of passive abilities happen through the
	 * destruction of the ability objects themselves (\c Ability).
	 * 
	 * Passive abilities are first added to an internal buffer, and will only
	 * activate in the prepareStep() bot phase. This is to ensure that programs
	 * running active abilities do not force abilities to start in the middle
	 * of the bot phase, unless the passive ability explicitly states so.
	 */
	class BotAbility : private Save::OperatorImpl<BotAbility> {
		public:
			BotAbility();
			~BotAbility();
			
			/**
			 * Minor class to manage available programs.
			 * This is also used by \c BotD and \c Player as both of
			 * those provide specific enabled programs for all bots for a
			 * given player or all bots of a given type.
			 * 
			 * \sa BotD
			 * \sa Player
			 */
			class AvailableProgram {
				public:
					AvailableProgram() {}
					~AvailableProgram() {}
					
					void clear()
					{ mEnabledProgram.clear(); }
					
					void enableProgram(IdType type)
					{ mEnabledProgram.set(type); }
					
					void mergeWith(const AvailableProgram &other)
					{	mEnabledProgram |= other.mEnabledProgram; }
					
					void excludeSet(const AvailableProgram &other)
					{	mEnabledProgram -= other.mEnabledProgram; }
					
					bool checkProgramEnabled(IdType type)
					{ return mEnabledProgram[type]; }
					
				private:
					typedef boost::dynamic_bitset<> EnabledProgramSet;
					EnabledProgramSet mEnabledProgram;
			};
			
			typedef std::list<IdType> AbilityList;
			
			bool checkProgramEnabled(IdType type)
			{ return mAvailProgram.checkProgramEnabled(type); }
			
			void addAbility(IdType id)
			{ mAbilityList.push_back(id); }
			
			void save(Save::BasePtr &fp) const;
			void load(Save::BasePtr &fp);
			
		private:
			void initialize(Bot *host);
			
			void prepareStep(double delta);
			void updateCpu(double delta);
			void step(double delta);
			
			void startAbility(Ability *ability);
			void endAbility(Ability *ability);
			
			template<class Func>
			void executeStepPart(Func f);
			
			template<class Func>
			void executeStepPartList(AbilityList &abl, Func f, bool canRemove=true);
			
			/// @name Constant/temporary data
			//@{
				Bot *mHost;
				AvailableProgram mAvailProgram;
			//@}
			
			/// @name Dynamic data
			//@{
				AbilityList mAbilityList;
				
				typedef boost::unordered_set<IdType> AbilityHasInitSet;
				AbilityHasInitSet mAbilityHasInit;
			//@}
			
			friend class Bot;
	};
}

#endif
