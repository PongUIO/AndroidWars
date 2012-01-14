#ifndef SIM_BOT_H
#define SIM_BOT_H

#include <stdint.h>
#include <boost/bind.hpp>

#include "Factory.h"
#include "Input.h"
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
	
	class Bot {
		public:
			struct Input {
				uint32_t mStepDelay;
				uint32_t mProgramId;
			};
			
			struct SensorState {
				uint32_t mTargetBot;
				
				bool mWasHit;
				
				void save(Save::BasePtr &fp);
				void load(Save::BasePtr &fp);
			};
			
			struct Engine {
				double mStrength;
				Vector mDirection;
			};
			
			struct State {
				public:
					State() : mSide(0), mType(0),
						mSensor(), mBody(), mWeapon(),
						mCpu(), mAbility(), mEngine()
						{}
					
					uint32_t mSide;
					uint32_t mType;
					
					SensorState mSensor;
					
					Body mBody;
					Health mHealth;
					
					BotWeapon mWeapon;
					BotCpu mCpu;
					BotAbility mAbility;
					Engine mEngine;
					
				private:
					void save(Save::BasePtr &fp);
					void load(Save::BasePtr &fp);
					
					friend class Bot;
			};
			typedef State Config;
			
			uint32_t getId() const { return mId; }
			uint32_t getTypeId() const { return getState().mType; }
			Body &getBody() { return getState().mBody; }
			Health &getHealth() { return getState().mHealth; }
			
			bool isDead() { return false; }
			
			State &getState() { return mState; }
			const State &getState() const { return mState; }
			
			/// @name Shorthand references to related data
			//@{
				const BotD *getTypePtr() const;
				Player *getPlayerPtr() const;
			//@}
			
		private:
			Bot(Simulation *sim, uint32_t id, const State &cfg=State());
			~Bot();
			
			/// @name Interaction
			//@{
				void prepareStep(double stepTime);
				void updateCpu(double stepTime);
				void step(double stepTime);
				
				void save(Save::BasePtr &fp);
				void load(Save::BasePtr &fp);
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
				uint32_t mId;
				Simulation *mSim;
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
				uint32_t createBot(const Bot::Config &cfg);
				
				Bot *getBot(uint32_t id) { return getObject(id); }
				
				const UidFactory<Bot>::DataList &getBotList() const
					{ return getData(); }
				
				void step(double stepTime);
				
				void startPhase();
				void endPhase();
			//@}
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Bot* obj) { delete obj; }
				void saveObj(Bot *bot, Save::BasePtr &fp);
				Bot* loadObj(uint32_t id, Save::BasePtr &fp);
			//@}
	};
	
}

#endif
