#ifndef SIM_PROGRAM_MOVETOWARDS_H
#define SIM_PROGRAM_MOVETOWARDS_H

#include "../Program.h"
#include "../Vector.h"

namespace Sim {
	namespace Prog {
		class MoveTowards : public Program {
			public:
				/// @name Save system implementation
				//@{
					static uint32_t getTypeId();
					class SaveSys : public Program::SaveSystem {
						public:
							Program* createProgram(Simulation *sim,
								uint32_t id) {
								return new MoveTowards(sim, id, Config());
							}
					};
				//@}
				
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
					DestinationType mType;
					uint32_t mTargetId;
					Vector mTargetPos;
					
					Config(DestinationType type=DtBotLock,
						   const Vector &pos=Vector(),
						   uint32_t id=FactoryNoId) :
						mType(type), mTargetId(id), mTargetPos(pos) {}
				};
				
				MoveTowards(Simulation *sim, uint32_t id, const Config &cfg);
				~MoveTowards();
				
				virtual uint32_t getCycleCost() { return 1; }
				
			private:
				virtual void save(Save::BasePtr& fp);
				virtual void load(Save::BasePtr& fp);
				
				virtual void process(Bot* bot, BotCpu* cpu);
				
				DestinationType mType;
				uint32_t mTarget;
				Vector mTargetPos;
		};
	}
}

#endif
