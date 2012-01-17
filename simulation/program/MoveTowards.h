#ifndef SIM_PROGRAM_MOVETOWARDS_H
#define SIM_PROGRAM_MOVETOWARDS_H

#include "../Program.h"
#include "../Vector.h"

namespace Sim {
	namespace Prog {
		/**
		 * Turns on a bot's engine to move it towards a destination.
		 * 
		 * @note This will eventually also take into consideration the type of
		 * engine a bot has. For example, if the engine only allows movement
		 * on ground, the program will not try to move it into the air.
		 */
		class MoveTowards : public Program {
			public:
				_SIM_PROGRAM_HEADER
				
				enum DestinationType {
					/// Locks onto a single bot based on the given target
					DtBotLock=0,
					
					/// Locks onto the active bot's information state target
					DtBotInfoLock,
					
					/// Move towards target bot, updates every step based
					/// on the bot information state.
					DtBotUpdate,
					
					/// Move towards target position
					DtPosition
				};
				
				struct Config {
					uint8_t mType;
					IdType mTargetId;
					Vector mTargetPos;
					
					Config(DestinationType type=DtBotLock,
						   const Vector &pos=Vector(),
						   IdType id=NoId) :
						mType(type), mTargetId(id), mTargetPos(pos) {}
				};
				
				MoveTowards(Simulation *sim, IdType id, IdType typeId,
					const Config &cfg);
				~MoveTowards();
				
				virtual uint32_t getCycleCost() { return 1; }
				
			private:
				virtual void save(Save::BasePtr& fp);
				virtual void load(Save::BasePtr& fp);
				
				virtual void process(Bot* bot, BotCpu* cpu);
				virtual bool isFinished(Bot*, BotCpu*) { return false; }
				
				uint8_t mType;
				IdType mTarget;
				Vector mTargetPos;
		};
	}
}

#endif
