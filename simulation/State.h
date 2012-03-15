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
//#include "Input.h"
#include "Replay.h"

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
			/// @name Initialization
			//@{
				State(Simulation *sim);
				~State();
				
				void operator=(const State &other);
				
				void startup();
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
				bool isInPhase() { return mCtrl.mInPhase; }
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
				
				struct StateControl :
				private Save::OperatorImpl<StateControl> {
					StateControl() : mCurPhaseStep(0), mCurPhase(0), mInPhase(false)
						{}
					
					uint32_t mCurPhaseStep;
					uint32_t mCurPhase;
					bool mInPhase;
					
					void save(Save::BasePtr &fp) const
					{ fp << mCurPhaseStep << mCurPhase << mInPhase; }
					void load(Save::BasePtr &fp)
					{ fp >> mCurPhaseStep >> mCurPhase >> mInPhase; }
				};
				StateControl mCtrl;
			//@}
			
			Simulation *mSim;
	};
}

#endif
