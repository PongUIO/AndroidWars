#ifndef SIM_STATE_H
#define SIM_STATE_H

#include <stdint.h>

#include "CommonTemplate.h"

#include "World.h"
#include "Bot.h"
#include "Bullet.h"
#include "Weapon.h"
#include "Player.h"
#include "Program.h"
#include "Ability.h"

#include "utility/CallGroup.h"
#include "StateObj.h"

namespace Sim {
	class Simulation;
	class BotFactory;
	class WeaponFactory;
	class BulletFactory;
	
#define _SIM_X_STATE_COMPONENTS \
	_SIM_X(BotFactory) \
	_SIM_X(ProgramFactory) \
	_SIM_X(AbilityFactory) \
	_SIM_X(BulletFactory) \
	_SIM_X(WeaponFactory) \
	_SIM_X(World) \
	_SIM_X(PlayerData) \
	
	class State : private CallGroup<StateObj> {
		public:
			/**
			 * Describes the current active state this simulation
			 * \c State object is in.
			 * 
			 * This is used to enforce and assure that \c State logic is
			 * performed in a reasonable manner. Violations of this
			 * behaviour will result in assertions being raised.
			 */
			enum StateType {
				/// \c State is in this state before \c startup and after
				/// \c shutdown has been called.
				/// 
				/// Valid calls: \c startup
				StUninitialized,
				
				/// \c State is in this state after \c startup and before
				/// \c shutdown has been called, and outside of any
				/// \c startPhase, \c step, and \c endPhase phase
				/// processing.
				/// 
				/// Valid calls: \c startPhase, \c shutdown, \c save
				StIdle,
				
				/// \c State is in this state after a call to
				/// \c startPhase, and there are still more calls to
				/// \c step before \c endPhase may be called.
				/// 
				/// Valid calls: \c step
				StInPhase,
				
				/// \c State is in this state after the final call to
				/// \c step during a phase (where the current state was
				/// \c StInPhase).
				/// 
				/// Valid calls: \c endPhase
				StEndPhase
			};
			
			/// @name Initialization
			//@{
				State(Simulation *sim);
				~State();
				
				void operator=(const State &other);
				
				void startup(uint32_t phaseLen);
				void shutdown();
			//@}
			
			/// @name Phase interface
			//@{
				void startPhase();
				void step(double stepTime);
				void endPhase();
			//@}
			
			/// @name State interface
			//@{
				void save(Save::BasePtr &fp);
				void load(Save::BasePtr &fp);
			//@}
			
			/// @name Module accessors
			//@{
				template<class T>
				T &getComponent();
				
				template<class T>
				typename T::FactoryType &getComponentByRelated()
				{ return getComponent<T::FactoryType>(); }
				
#define _SIM_X(type) type &get##type() { return m##type; }
				_SIM_X_STATE_COMPONENTS
#undef _SIM_X

				uint32_t getCurPhaseStep() { return mCtrl.mCurPhaseStep; }
				uint32_t getCurPhase() { return mCtrl.mCurPhase; }
				StateType getStateType() { return StateType(curState()); }
			//@}
			
		private:
			/// @name State objects
			//@{
				void registerStateObj();
				
#define _SIM_X(type) type m##type;
				_SIM_X_STATE_COMPONENTS
#undef _SIM_X
			//@}
			
			/// @name State control
			//@{
				uint32_t &curPhaseStep() { return mCtrl.mCurPhaseStep; }
				uint32_t &curPhase() { return mCtrl.mCurPhase; }
				uint32_t &curState() { return mCtrl.mCurState; }
				uint32_t &phaseLength() { return mCtrl.mPhaseLength; }
				
				struct StateControl :
				private Save::OperatorImpl<StateControl> {
					StateControl() : mCurPhaseStep(0), mCurPhase(0),
						mPhaseLength(0), mCurState(StUninitialized)
						{}
					
					uint32_t mCurPhaseStep;
					uint32_t mCurPhase;
					uint32_t mPhaseLength;
					uint32_t mCurState;
					
					void save(Save::BasePtr &fp) const
					{ fp << mCurPhaseStep << mCurPhase << mPhaseLength
						<< mCurState; }
					void load(Save::BasePtr &fp)
					{ fp >> mCurPhaseStep >> mCurPhase >> mPhaseLength
						>> mCurState; }
				};
				StateControl mCtrl;
			//@}
			
			Simulation *mSim;
	};
}

#endif
