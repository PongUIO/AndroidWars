#ifndef SIM_BOT_H
#define SIM_BOT_H

#include <stdint.h>

#include "Factory.h"
#include "Input.h"
#include "Body.h"
#include "Save.h"

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
				
				Vector pos;
			};
			
			uint32_t getId() const { return mId; }
			const Body &getBody() const { return mBody; }
			
		private:
			Bot(Simulation *sim, uint32_t id, const Config &cfg);
			~Bot();
			
			/// @name Interaction
			//@{
				bool isDead() { return false; }
				void step(double stepTime);
				
				void save(Save::BasePtr &fp);
			//@}
			
			/// @name Identification
			//@{
				uint32_t mId;
				uint32_t mSide;
				uint32_t mType;
			//@}
			
			/// @name Input
			//@{
				InputBuffer<BotInput> mInput;
				BotInput mCurInput;
				
				bool isIdle() {
					return mCurInput.type==BotInput::None ||
						mCurInput.stepCount==0;
				}
				
				void handleInput();
			//@}
			
			/// @name Physical
			//@{
				Body mBody;
				
				const BotD *mTypePtr;
				Simulation *mSim;
			//@}
			
			friend class BotFactory;
			friend class Factory<Bot>;
	};
	
	class BotFactory : public Factory<Bot> {
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
				
				/// @name State
				//@{
					void copyState(State &state);
					
					void save(Save::BasePtr &fp);
				//@}
			//@}
			
		private:
			void deleteInstance(Bot *obj) { delete obj; }
			Bot *newCopyInstance(Bot *obj) { return new Bot(*obj); }
			
			InputBuffer<BotInput> mInput;
			
	};
	
}

#endif
