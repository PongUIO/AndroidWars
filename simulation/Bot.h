#ifndef SIM_BOT_H
#define SIM_BOT_H

#include <stdint.h>
#include <boost/bind.hpp>

#include "Factory.h"
#include "Body.h"
#include "Health.h"
#include "Save.h"
#include "Weapon.h"

#include "bot/BotCpu.h"
#include "bot/BotAbility.h"
#include "bot/BotWeapon.h"

#include "StateObj.h"

namespace Sim {
	// Forward declarations
	class Simulation;
	class BotFactory;
	class Bot;
	class BotD;
	class State;
	class Collision;
	class Player;
	class BotDatabase;
	
	/**
	 * Base class for bot types
	 */
	class Bot {
		public:
			typedef BotDatabase TypeDatabase;
			
			/**
			 * Contains information about the state of bot "sensors".
			 * 
			 * These are various variables that aid a bot in knowing what
			 * happens around it, whether it was hit the previous step, etc.
			 */
			struct SensorState : private Save::OperatorImpl<SensorState> {
				IdType mTargetBot;
				bool mWasHit;
				
				void save(Save::BasePtr &fp) const
					{ fp << mTargetBot << mWasHit; }
				void load(Save::BasePtr &fp)
					{ fp >> mTargetBot >> mWasHit; }
			};
			
			/**
			 * Contains temporary information about the bot's
			 * planned movement.
			 */
			struct Engine {
				double mStrength;
				Vector mDirection;
			};
			
			struct State : private Save::OperatorImpl<State> {
				public:
					State() : mSide(0),
						mSensor(), mBody(), mWeapon(),
						mCpu(), mAbility(), mEngine()
						{}
					
					IdType mType;
					IdType mSide;
					
					SensorState mSensor;
					Body mBody;
					Health mHealth;
					
					BotWeapon mWeapon;
					BotCpu mCpu;
					BotAbility mAbility;
					Engine mEngine;
					
					void save(Save::BasePtr &fp) const;
					void load(Save::BasePtr &fp);
			};
			typedef State Config;
			
			IdType getId() const { return mId; }
			IdType getTypeId() const { return mTypeId; }
			
			State &getState() { return mState; }
			const State &getState() const { return mState; }
			Body &getBody() { return getState().mBody; }
			Health &getHealth() { return getState().mHealth; }
			
			bool isDead() { return mDoRemove; }
			
			const BotD *getTypePtr() const;
			Player *getPlayerPtr() const;
			
			Bot(Simulation *sim, IdType id, IdType typeId, const State &cfg=State());
			~Bot();
			
		protected:
			/// @name Interaction
			//@{
				virtual void save(Save::BasePtr &fp) const;
				virtual void load(Save::BasePtr &fp);
				
				virtual void prepareStep(double stepTime);
				virtual void updateCpu(double stepTime);
				virtual void step(double stepTime);
				
				void checkDeath();
				bool isIdle() { return mState.mCpu.getProgramList().size()==0; }
				void handleInput();
			//@}
			
			/**
			 * @name System data
			 * Contains data for a bot that is
			 * used for management of a bot.
			 */
			//@{
				// Core data
				IdType mId;
				IdType mTypeId;
				Simulation *mSim;
				
				// State flags
				bool mDoRemove;
			//@}
			
			/**
			 * @name Simulation data
			 * Contains data that is directly related
			 * to simulation. All these values are used in saving/loading.
			 */
			State mState;
			
			friend class DefaultUidFactory<Bot>;
			friend class BotFactory;
			
			friend class BotCpu;
			friend class BotAbility;
			friend class BotWeapon;
	};
	
	class BotFactory : public DefaultUidFactory<Bot> {
		public:
			BotFactory(Simulation *sim);
			virtual ~BotFactory();
			
			/// @name StateObj functions
			//@{
				void startup();
				void shutdown();
				
				void save(Save::BasePtr& fp);
				void load(Save::BasePtr& fp);
				
				void startPhase();
				void endPhase();
				
				void step(double stepTime);
			//@}
			
			Bot *getBot(IdType id) { return getObject(id); }
			
			/// @name Compatibility
			//@{
				//IdType createBot(const Bot::Config &cfg);
				const UidFactory<Bot>::DataList &getBotList() const
					{ return getData(); }
			//@}
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Bot* obj) { delete obj; }
			//@}
	};
}

#endif
