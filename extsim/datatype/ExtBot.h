#ifndef EXTSIM_EXTBOT_H
#define EXTSIM_EXTBOT_H

#include <stdint.h>
#include <vector>

#include "../ExtBaseData.h"

#include "../../simulation/Health.h"

namespace Sim
{ class BotD; class ArmorDatabase; }

namespace ExtS {
	class ExtBotData;
	
	class ExtBot : public ExtBaseDataObj {
		public:
			struct WeaponSlot {
				WeaponSlot(const std::string &type) : mType(type) {}
				
				std::string mType;
			};
			
			ExtBot(ExtSim *esim);
			~ExtBot();
			
			void loadNode(DaScript::Node &node,
				Sim::IdType simTypeId, TypeRule *rule);
			void postProcess(ExtSim &extsim);
			
			uint32_t getBaseCost() const { return mBaseCost; }
			
		private:
			void loadSimBot(DaScript::Node &node, Sim::BotD *simBot);
			
			/// @name Extended data
			//@{
				uint32_t mBaseCost;
			//@}
			
			typedef std::vector<WeaponSlot> WeaponSlotVec;
			WeaponSlotVec mWeaponSlot;
			
			/// @name Temporary data
			//@{
				struct HealthHull : public Sim::Health::Hull {
					std::string mType;
					
					void loadData(DaScript::Node &data);
					void postProcess(Sim::ArmorDatabase &armorDb);
				};
				typedef std::vector<HealthHull> AttachmentVec;
				
				
				HealthHull mCoreHealth;
				AttachmentVec mAttachmentHealth;
			//@}
	};
	
	class ExtBotData : public DefaultExtData<ExtBot> {
		public:
			ExtBotData(ExtSim &esim);
			virtual ~ExtBotData();
			
			void startup() {}
			void shutdown() {}
			
	};
}

#endif
