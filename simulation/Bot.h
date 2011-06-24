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
	class State;
	
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
				
				Vector pos;
			};
			
			uint32_t getId() const { return mId; }
			const Body &getBody() const { return mBody; }
			
		private:
			Bot(uint32_t id, const Config &cfg);
			~Bot();
			
			/// @name Interaction
			//@{
				bool isDead() { return false; }
				void step(double stepTime);
				
				void save(Save::Ptr &fp);
			//@}
			
			/// @name Identification
			//@{
				uint32_t mId;
				uint32_t mSide;
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
				
				void startPhase();
				void endPhase();
				
				InputBuffer<BotInput> &getInput() { return mInput; }
				
				/// @name State
				//@{
					void save(Save::Ptr &fp);
					void copyState(State &state);
				//@}
			//@}
			
		private:
			void deleteInstance(Bot *obj) { delete obj; }
			Bot *newCopyInstance(Bot *obj) { return new Bot(*obj); }
			
			InputBuffer<BotInput> mInput;
			
	};
	
}

#endif
