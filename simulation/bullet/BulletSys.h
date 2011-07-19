#ifndef SIM_BULLETSYS_H
#define SIM_BULLETSYS_H

#include <vector>
#include <stdint.h>

namespace Sim {
	// Forward declarations
	class Simulation;
	
	/**
	 * Defines a bullet state system.
	 * 
	 * This system stores behaviours for a bullet state
	 */
	class BulletSys {
		public:
			class State;
			class Thread;
			class Reference;
			typedef std::vector<State*> StateVec;
			typedef std::vector<uint32_t> EntryPointVec;
			
			/**
			 * Defines a reference to a bullet state system.
			 * 
			 * This holds variable data used by a state system.
			 */
			class Reference {
				public:
					Reference(Simulation *sim, BulletSys *sys);
					~Reference();
					
					void startThread(uint32_t entryId);
					
				private:
					/**
					 * Defines a thread.
					 * 
					 * Threads are execution states for system
					 * references.
					 */
					struct Thread {
						State *mActive;
					};
					
					Simulation *mSim;
					BulletSys *mSystem;
					State *mActive;
					
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
					
					/**
					 * Executes this state for the input reference.
					 */
					virtual void exec(Reference &ref)=0;
					
					size_t insertChild(State *child)
					{
						mStates.push_back(child);
						return mStates.size()-1;
					}
					
				protected:
					void nextState(Reference &ref, size_t id=0)
					{ ref.setActive(mStates[id]); }
					
					StateVec mStates;
					
				private:
					/**
					 * Initializes this state, called by \c BulletSys
					 * when it is initialized.
					 */
					virtual void init(BulletSys &sys) {}
			};
			
			// Functions
			BulletSys();
			~BulletSys();
			
			/// Performs finishing touches when the system
			/// is fully created.
			void finalize();
			
			/// Registers a bullet state and returns its ID.
			uint32_t registerState(State *state);
			void registerEntryPoint(uint32_t entry);
			
			/**
			 * Sets the root state for this state system.
			 * 
			 * The root state is the entry-node where new references
			 * start processing.
			 */
			void setRoot(State *root) { mRoot = root; }
			State *getRoot() { return mRoot; }
			
			State *getState(size_t id) { return mStates.at(id); }
			
		private:
			State *mRoot;
			StateVec mStates;
			EntryPointVec mEntry;
			
			friend class State;
	};
	
	// Default bullet system behaviours
	namespace BSys {
		/// @note Unfinished implementation
		class Delay : public BulletSys::State {
			public:
				void exec(BulletSys::Reference &ref ) {
					nextState(ref);
				}
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
