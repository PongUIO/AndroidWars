#ifndef SIM_BOT_H
#define SIM_BOT_H

#include <stdint.h>

#include "Factory.h"
#include "Input.h"
#include "Body.h"

namespace Sim {
	// Forward declarations
	class Simulation;
	class BotFactory;
	class Sync;
	class State;
	
	struct BotInput {
		enum InputType {
			None=0,
			Move
		};
		
		BotInput(uint32_t id=0) :
			botId(id), stepCount(0), type(None)
			{}
		
		uint32_t botId;      ///< ID of bot to process this input
		uint32_t stepCount;  ///< Number of steps to perform the action
		InputType type;     ///< Type of action to perform
		
		struct MoveP {
			Vector dir;
		};
		
		MoveP move;
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
				
				void checksum(Sync &sync);
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
					void checksum(Sync &sync);
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
