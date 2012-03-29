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
			
			ExtBot();
			~ExtBot();
			
			void loadBlock(Script::Block &block, TypeRule *rule);
			void postProcess(ExtSim &extsim);
			
			//const std::string &getName() const { return mName; }
			//const std::string &getDescription() const { return mDescription; }
			
			uint32_t getBaseCost() const { return mBaseCost; }
			//Sim::IdType getId() const { return mId; }
			
		private:
			/// @name Extended data
			//@{
				/*Sim::IdType mId;
				
				std::string mName;
				std::string mDescription;*/
				
				uint32_t mBaseCost;
			//@}
			
			typedef std::vector<WeaponSlot> WeaponSlotVec;
			WeaponSlotVec mWeaponSlot;
			
			/// @name Temporary data
			//@{
				Sim::BotD *mSimData;
				
				struct HealthHull : public Sim::Health::Hull {
					std::string mType;
					
					void loadData(Script::Data &data);
					void postProcess(Sim::ArmorDatabase &armorDb);
				};
				typedef std::vector<HealthHull> AttachmentVec;
				
				
				HealthHull mCoreHealth;
				AttachmentVec mAttachmentHealth;
			//@}
			
			friend class ExtBotData;
			friend class Sim::DataCtr<ExtBot>;
	};
	
	class ExtBotData : public DefaultExtData<ExtBot> {
		public:
			ExtBotData(ExtSim &esim);
			virtual ~ExtBotData();
			
			void startup() {}
			void shutdown() {}
			
			void setupObject(Script::Block& block,
				TypeRule* rule, ExtBot* obj);
			
		private:
			void loadSimBot(Script::Block& block, Sim::BotD *simBot);
			
			friend class ExtBot;
	};
}

#endif
