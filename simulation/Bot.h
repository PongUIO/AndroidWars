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
	
	class Bot {
		public:
			typedef BotDatabase TypeDatabase;
			
			struct Input {
				uint32_t mStepDelay;
				IdType mProgramId;
			};
			
			struct SensorState : private Save::OperatorImpl<SensorState> {
				IdType mTargetBot;
				bool mWasHit;
				
				void save(Save::BasePtr &fp) const
					{ fp << mTargetBot << mWasHit; }
				void load(Save::BasePtr &fp)
					{ fp >> mTargetBot >> mWasHit; }
			};
			
			struct Engine {
				double mStrength;
				Vector mDirection;
			};
			
			struct State : private Save::OperatorImpl<State> {
				public:
					State() : mSide(0), mType(0),
						mSensor(), mBody(), mWeapon(),
						mCpu(), mAbility(), mEngine()
						{}
					
					IdType mSide;
					IdType mType;
					
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
			
			Bot(Simulation *sim, IdType id, const State &cfg=State());
			~Bot();
			
			IdType getId() const { return mId; }
			IdType getTypeId() const { return getState().mType; }
			Body &getBody() { return getState().mBody; }
			Health &getHealth() { return getState().mHealth; }
			
			bool isDead() { return mDoRemove; }
			
			State &getState() { return mState; }
			const State &getState() const { return mState; }
			
			/// @name Shorthand references to related data
			//@{
				const BotD *getTypePtr() const;
				Player *getPlayerPtr() const;
			//@}
			
		private:
			/// @name Interaction
			//@{
				void prepareStep(double stepTime);
				void updateCpu(double stepTime);
				void step(double stepTime);
				
				void save(Save::BasePtr &fp) const;
				void load(Save::BasePtr &fp);
				
				void checkDeath();
			//@}
			
			/// @name Input
			//@{
				bool isIdle() {
					return mState.mCpu.getProgramList().size()==0;
				}
				
				void handleInput();
			//@}
			
			/**
			 * @name System data
			 * Contains data for a bot that is
			 * used for management of a bot.
			 */
			//@{
				IdType mId;
				Simulation *mSim;
				
				bool mDoRemove;
			//@}
			
			/**
			 * @name Simulation data
			 * Contains data that is directly related
			 * to simulation. All these values are used in saving/loading.
			 */
			State mState;
			
			friend class BotFactory;
			friend class DefaultUidFactory<Bot>;
			friend class BotCpu;
			friend class BotAbility;
			friend class BotWeapon;
	};
	
	class BotFactory : public DefaultUidFactory<Bot> {
		public:
			/// @name Initialization
			//@{
				BotFactory(Simulation *sim);
				virtual ~BotFactory();
				
				void startup();
				void shutdown();
				
				void save(Save::BasePtr& fp);
				void load(Save::BasePtr& fp);
			//@}
			
			/// @name Interaction
			//@{
				IdType createBot(const Bot::Config &cfg);
				
				Bot *getBot(IdType id) { return getObject(id); }
				
				const UidFactory<Bot>::DataList &getBotList() const
					{ return getData(); }
				
				void step(double stepTime);
				
				void startPhase();
				void endPhase();
			//@}
			
			static BotFactory &getFactory(Simulation *sim);
			
			void saveObj(Bot *bot, Save::BasePtr &fp);
			Bot* loadObj(IdType id, Save::BasePtr &fp);
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Bot* obj) { delete obj; }
			//@}
	};
	
}

#endif
