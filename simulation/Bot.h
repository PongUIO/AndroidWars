#ifndef SIM_BOT_H
#define SIM_BOT_H

#include <stdint.h>
#include <boost/bind.hpp>

#include "Factory.h"
#include "Input.h"
#include "Body.h"
#include "Save.h"
#include "Weapon.h"

#include "StateObj.h"

namespace Sim {
	// Forward declarations
	class Simulation;
	class BotFactory;
	class Bot;
	class BotD;
	class State;
	class Collision;
	
	struct BotInput {
		public:
			enum InputType {
				None=0,
				Move,
				Shoot,
				Ability
			};
			
			BotInput(uint32_t id=0, InputType type=None) :
				botId(id), stepCount(0), type(type), dir(0)
				{}
			
			static BotInput inMove(uint32_t id, uint32_t stepCount,
				const Vector &dir) {
				BotInput tmp = BotInput(id, Move);
				tmp.stepCount = stepCount;
				tmp.dir = dir;
				
				return tmp;
			}
			
			static BotInput inShoot(uint32_t id, uint32_t weapId,
				const Vector &dir) {
				BotInput tmp = BotInput(id, Shoot);
				tmp.stepCount = 1;
				tmp.dir = dir;
				
				tmp.iparam[0] = weapId;
				
				return tmp;
			}
		
		private:
			uint32_t botId;      ///< ID of bot to process this input
			uint32_t stepCount;  ///< Number of steps to perform the action
			InputType type;      ///< Type of action to perform
			
			Vector dir;          ///< Direction to apply the action
			
			int32_t iparam[3];
			double dparam[3];
			Vector vparam[3];
			
			friend class Bot;
			friend class BotFactory;
	};
	
	class Bot {
		public:
			struct Config {
				uint32_t side;
				uint32_t type;
				
				Body body;
				WeaponBox weaponBox;
				
				InputBuffer<BotInput> input;
				BotInput curInput;
			};
			
			uint32_t getId() const { return mId; }
			const Body &getBody() const { return mBody; }
			
		private:
			Bot(Simulation *sim, uint32_t id) : mId(id), mSim(sim) {}
			Bot(Simulation *sim, uint32_t id, const Config &cfg);
			~Bot();
			
			/// @name Interaction
			//@{
				bool isDead() { return false; }
				void step(double stepTime);
				
				void save(Save::BasePtr &fp);
				void load(Save::BasePtr &fp);
				
				void loadConfig(const Config &cfg);
			//@}
			
			/// @name Input
			//@{
				bool isIdle() {
					return mCurInput.type==BotInput::None ||
						mCurInput.stepCount==0;
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
				
				const BotD *mTypePtr;
				Simulation *mSim;
			//@}
			
			/**
			 * @name Simulation data
			 * Contains data that is directly related
			 * to simulation. All these values are used in saving/loading.
			 */
			//@{
				uint32_t mSide;
				uint32_t mType;
				
				Body mBody;
				WeaponBox mWeaponBox;
				
				InputBuffer<BotInput> mInput;
				BotInput mCurInput;
			//@}
			
			friend class BotFactory;
			friend class Factory<Bot>;
			friend class DefaultFactory<Bot>;
	};
	
	class BotFactory : public DefaultFactory<Bot> {
		public:
			/// @name Initialization
			//@{
				BotFactory(Simulation *sim);
				~BotFactory();
				
				void startup();
				void shutdown();
			//@}
			
			/// @name Interaction
			//@{
				uint32_t createBot(const Bot::Config &cfg);
				
				const Bot *getBot(uint32_t id) const {
					return getObject(id);
				}
				
				const Factory<Bot>::ObjVec &getBotVector() const {
					return mData;
				}
				
				void startPhase();
				void endPhase();
				
				InputBuffer<BotInput> &getInput() { return mInput; }
			//@}
			
		private:
			void deleteInstance(Bot *obj) { delete obj; }
			
			InputBuffer<BotInput> mInput;
	};
	
}

#endif
