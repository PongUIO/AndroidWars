#ifndef SIM_ABILITY_ARMORATTACHMENT_H
#define SIM_ABILITY_ARMORATTACHMENT_H

#include "../Ability.h"
#include "../Health.h"

namespace Sim { namespace Abil {
	/**
	 * Creates or appends an armor attachment for a bot.
	 * 
	 * The armor is added when the bot processes this ability for the first
	 * time, and removed when processed for the last time.
	 */
	class ArmorAttachment : public Ability {
		public:
			_SIM_ABILITY_HEADER
			
			class Config : public Health::Hull,
					private Save::OperatorImpl<Config> {
				public:
					Config(IdType type=0, uint32_t maxHealth=0) :
						Hull(type, maxHealth) {}
					
					/*void save(Save::BasePtr &fp) const
					{ fp << (Health::Hull&)*this; }
					void load(Save::BasePtr &fp)
					{ fp >> (Health::Hull&)*this; }*/
			};
			
			ArmorAttachment(Simulation* sim, IdType id, IdType typeId,
				const Config &cfg);
			virtual ~ArmorAttachment();
			
		private:
			void save(Save::BasePtr &fp);
			void load(Save::BasePtr &fp);
			
			void prepareStep(double, Bot*) {}
			void updateCpu(double, Bot*) {}
			void step(double, Bot*) {}

			void start(Bot *bot);
			void end(Bot *bot);
			
			Config mData;
	};
} }

#endif
