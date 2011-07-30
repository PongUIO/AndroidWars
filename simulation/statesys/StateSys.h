#ifndef SIM_STATESYS_H
#define SIM_STATESYS_H

#include <boost/any.hpp>

#include <vector>
#include <list>
#include <stdint.h>

#include "../Save.h"

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
						Thread(Reference *ref=0, const State *state=NULL,
							   IdType type=0,
							   const boost::any &arg=boost::any());
						
						Reference *mHost;
						const State *mActive;
						double mDelay;
						IdType mEntryType;
						boost::any mArg;
					};
					typedef std::list<Thread> ThreadList;
					
					/**
					 * Class that handles saving/loading of
					 * thread-specific values.
					 */
					struct ArgSave {
						virtual void save(Save::BasePtr &fp,
							const Thread &ref
						)const =0;
						virtual void load(Save::BasePtr &fp,
							Thread &ref
						)const =0;
					};
					
					// Functions
					Reference(const StateSys *sys=NULL);
					~Reference();
					
					// Saving/loading
					void save(Save::BasePtr &fp, const ArgSave &argSave);
					void load(Save::BasePtr &fp, const ArgSave &argSave,
							  StateSys *sys);
					
					void startThread(
						IdType entryId,
						const boost::any &arg=boost::any()
						);
					void exec(double delta);
					
				private:
					const StateSys *mSystem;
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
					virtual void exec(Reference::Thread &thread) const =0;
					
					IdType insertChild(State *child);
					IdType getId() const { return mId; }
					
				protected:
					const State *nextState(IdType id=0) const
					{ if(id<0 || id>=mStates.size()) return NULL;
					  else return mStates[id]; }
					
					StateVec mStates;
					IdType mId;
					
				private:
					void setId(IdType id) { mId=id; }
					
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
			const State *getState(IdType id) const { return mStates.at(id); }
			const State *getEntry(IdType entryId) const
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
				
				void exec(StateSys::Reference::Thread &thread ) const {
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
