#include "Bot.h"

#include "../../simulation/Simulation.h"

namespace ExtS {
	// BotData
	//
	//
	BotData::BotData(Sim::Simulation& sim): DefaultData<ExtBot>(sim)
	{}
	
	BotData::~BotData()
	{}
	
	void BotData::loadBlock(Script::Block& block)
	{
		const std::string &name = block.getDataFirst("Name");
		if(name.empty())
			return;
		
		// Create the sim/extsim data pair
		Sim::BotD *simData = mSim.getData().getBotDb().createType();
		ExtBot *extData = createType();
		extData->mSimData = simData;
		
		// Fill extsim with data
		extData->loadBlock(*this, block);
		
		// Fill core simulation data
		loadSimBot(block, simData);
	}
	
	void BotData::loadSimBot(Script::Block& block, Sim::BotD *simBot)
	{
		simBot->baseSpeed = readValue<double>("Speed", 0);
	}
	
	void BotData::postProcess()
	{
		for(DataVec::iterator i=mData.begin(); i!=mData.end(); ++i) {
			(*i)->postProcess(*this, mSim);
		}
	}

	
	// ExtBot
	//
	//
	ExtBot::ExtBot() {}
	ExtBot::~ExtBot() {}
	
	void ExtBot::loadBlock(BotData &host, Script::Block& block)
	{
		// Load extended data
		{
			mName = block.getDataFirst("Name");
			mDescription = block.getDataFirst("Description");
			
			mBaseCost = host.readValue<uint32_t>(
				block.getDataFirst("BaseCost"), 0);
			
			// Load weapon slots
			Script::Block *slotBlock = block.getBlock("WEAPONS");
			for(size_t i=0; i<slotBlock->getSize(); i++) {
				Script::Data *slotData = slotBlock->getDataIndex(i);
				
				if(slotData->getId() == "Slot") {
					const std::string &slotType = slotData->getArg(0);
					int slotCount = host.readValue<int>(slotData->getArg(1), 1);
					
					for(int sc=0; sc<slotCount; sc++)
						mWeaponSlot.push_back( WeaponSlot(slotType) );
				}
			}
		}
		
		// Load temporary data
		{
			// Load attachments
			mCoreHealth.loadData(host, block.getDataSimple("Health"));
			
			Script::Block *atmBlock = block.getBlock("ATTACHMENTS");
			if(atmBlock) {
				for(size_t hi=0; hi<atmBlock->getSize(); ++hi) {
					Script::Data *healthData = atmBlock->getDataIndex(hi);
					if(healthData) {
						mAttachmentHealth.push_back(HealthHull());
						mAttachmentHealth.back().loadData(host, *healthData);
					}
				}
			}
		}
	}
	
	void ExtBot::postProcess(BotData& host, Sim::Simulation& sim)
	{
		Sim::ArmorDatabase &armorDb = sim.getData().getArmorDb();
		
		mCoreHealth.postProcess(armorDb);
		mSimData->coreHealth.getCore() = mCoreHealth;
		
		for(AttachmentVec::iterator i=mAttachmentHealth.begin();
			i!=mAttachmentHealth.end(); ++i) {
			(*i).postProcess(armorDb);
			
			mSimData->coreHealth.addAttachment(*i);
		}
	}

	// ExtBot::HealthHull
	//
	//
	void ExtBot::HealthHull::loadData(BotData &host, Script::Data& data)
	{
		mType = data.getArg(0);
		
		int health, maxHealth;
		health = maxHealth = host.readValue<int>(data.getArg(1), 1);
		if(data.argCount()==3)
			health = host.readValue<int>(data.getArg(2), maxHealth);
		
		addMaxHealth(maxHealth, false);
		addHealth(health);
	}
	
	void ExtBot::HealthHull::postProcess(Sim::ArmorDatabase& armorDb)
	{
		setType(armorDb.getIdOf(mType));
	}
}