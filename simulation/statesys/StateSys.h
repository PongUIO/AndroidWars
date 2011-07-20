#ifndef SIM_STATESYS_H
#define SIM_STATESYS_H

#include <boost/any.hpp>

#include <vector>
#include <list>
#include <stdint.h>

namespace Sim {
	// Forward declarations
	class Simulation;
	
	/**
	 * Defines a state system.
	 */
	class StateSys {
		public:
			class State;
			class Reference;
			typedef uint32_t IdType;
			typedef std::vector<State*> StateVec;
			typedef std::vector<IdType> IdVec;
			
			/**
			 * Defines a reference to a bullet state system.
			 * 
			 * This holds variable data used by a state system.
			 */
			class Reference {
				public:
					/**
					 * Defines a thread.
					 * 
					 * Threads are execution states for system
					 * references.
					 */
					struct Thread {
						Thread(Reference *ref=0, State *state=NULL,
							   IdType type=0,
							   const boost::any &arg=boost::any());
						
						Reference *mHost;
						State *mActive;
						double mDelay;
						IdType mEntryType;
						boost::any mArg;
					};
					typedef std::list<Thread> ThreadList;
					
					// Functions
					Reference(StateSys *sys);
					~Reference();
					
					void startThread(
						IdType entryId,
						const boost::any &arg=boost::any()
						);
					void exec(double delta);
					
				private:
					StateSys *mSystem;
					ThreadList mThreads;
					
					friend class Thread;
			};
			
			/**
			 * Defines a bullet state.
			 * Each bullet state is associated with a specific behaviour.
			 */
			class State {
				public:
					State() {}
					virtual ~State() {}
					
					/// Executes this state for the input thread.
					virtual void exec(Reference::Thread &thread)=0;
					
					IdType insertChild(State *child);
					
				protected:
					State *nextState(IdType id=0)
					{ return mStates.at(id); }
					
					StateVec mStates;
					
				private:
					/// Initializes this state, called by \c StateSys
					/// when it is initialized.
					virtual void init(StateSys &sys) {}
					
					friend class StateSys;
			};
			
			// Functions
			StateSys();
			~StateSys();
			
			/// Performs finishing touches when the system
			/// is fully created.
			void finalize();
			
			/// Registers a bullet state and returns its ID.
			IdType registerState(State *state);
			
			/// Registers a state as an entry point
			/// @return The id of this entry point
			IdType registerEntryPoint(IdType entry);
			
			State *getState(IdType id) { return mStates.at(id); }
			State *getEntry(IdType entryId)
			{ return mStates.at(mEntry.at(entryId)); }
			
		private:
			StateVec mStates;
			IdVec mEntry;
			
			friend class State;
	};
	
	// Default bullet system behaviours
	namespace StdState {
		/// @note Unfinished implementation
		class Delay : public StateSys::State {
			public:
				Delay(double t) : mTime(t) {}
				
				void exec(StateSys::Reference::Thread &thread ) {
					thread.mDelay += mTime;
					thread.mActive = nextState();
				}
				
			private:
				double mTime;
		};
		
		/// @note Currently implemented states:
		
		/// @todo Future state implementations:
		/// - Delay     : Delays the next state call
		/// - Die       : Kills the active bullet
		/// - BulletFan : Creates a fan of bullets
		/// - ModBody   : Modifies the bullet's body
		/// 
		/// Possible advanced implementations:
		/// (some of these require a system for variables)
		/// - Fork      : Creates an exact replica of the bullet,
		///   where each execute from separate child states.
		/// - Conditional : Executes child states based on a conditional
	}
}

#endif
