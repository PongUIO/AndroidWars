#ifndef EXTSIM_BOT_H
#define EXTSIM_BOT_H

#include <stdint.h>
#include <vector>

#include "../BaseData.h"

#include "../../simulation/Health.h"

namespace Sim
{ class BotD; class ArmorDatabase; }

namespace ExtS {
	class BotData;
	
	class ExtBot {
		public:
			struct WeaponSlot {
				WeaponSlot(const std::string &type) : mType(type) {}
				
				std::string mType;
			};
			
			ExtBot();
			~ExtBot();
			
			void loadBlock(BotData &host, Script::Block &block);
			void postProcess(BotData &host, Sim::Simulation &sim);
			
			const std::string &getName() const { return mName; }
			const std::string &getDescription() const { return mDescription; }
			
			uint32_t getBaseCost() const { return mBaseCost; }
			
		private:
			/// @name Extended data
			//@{
				Sim::IdType mId;
				
				std::string mName;
				std::string mDescription;
				
				uint32_t mBaseCost;
			//@}
			
			typedef std::vector<WeaponSlot> WeaponSlotVec;
			WeaponSlotVec mWeaponSlot;
			
			/// @name Temporary data
			//@{
				Sim::BotD *mSimData;
				
				struct HealthHull : public Sim::Health::Hull {
					std::string mType;
					
					void loadData(BotData &host, Script::Data &data);
					void postProcess(Sim::ArmorDatabase &armorDb);
				};
				typedef std::vector<HealthHull> AttachmentVec;
				
				
				HealthHull mCoreHealth;
				AttachmentVec mAttachmentHealth;
			//@}
			
			friend class BotData;
			friend class Sim::DataCtr<ExtBot>;
	};
	
	class BotData : public DefaultData<ExtBot> {
		public:
			BotData(ExtSim &esim);
			virtual ~BotData();
			
			void startup() {}
			void shutdown() {}
			
			void loadBlock(Script::Block& block);
			void postProcess();
			
		private:
			void loadSimBot(Script::Block& block, Sim::BotD *simBot);
			
			friend class ExtBot;
	};
}

#endif
