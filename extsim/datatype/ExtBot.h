#ifndef EXTSIM_EXTBOT_H
#define EXTSIM_EXTBOT_H

#include <stdint.h>
#include <vector>

#include "../DefaultExtData.h"
#include "../object/ExtDataObj.h"

#include "../../simulation/Health.h"

namespace Sim
{ class BotD; class ArmorDatabase; }

namespace exts {
	class ExtBotData;
	
	class ExtBot : public ExtDataObjBase {
		public:
			struct WeaponSlot {
				WeaponSlot(const std::string &type) : mType(type) {}
				
				std::string mType;
			};
			
			ExtBot(ExtSim &esim, Sim::IdType id);
			~ExtBot();
			
			void loadNode(const Nepeta::Node &node);
			void postProcess();
			
			uint32_t getBaseCost() const { return mBaseCost; }
			
		private:
			void loadSimBot(const Nepeta::Node &node, Sim::BotD *simBot);
			
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
					
					void loadData(const Nepeta::Node &data);
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
			
		private:
			
			
			friend class ExtBot;
	};
}

#endif
