#include "ExtBot.h"
#include "../ExtData.h"

#include "../../simulation/Simulation.h"
#include "../ExtSim.h"

namespace ExtS {
	// ExtBotData
	//
	//
	ExtBotData::ExtBotData(ExtSim &esim): DefaultExtData<ExtBot>(esim)
	{
		mIgnoreRule=true;
	}
	
	ExtBotData::~ExtBotData()
	{}
	
	void ExtBotData::setupObject(Script::Block& block,
		TypeRule* rule, ExtBot* obj)
	{
		// Create the corresponding sim object
		Sim::BotD *simData =
			mExtSim->getSim().getData().getBotDb().createType();
		
		obj->mSimData = simData;
		obj->loadBlock(block, 0);
		
		loadSimBot(block, simData);
	}

	
	void ExtBotData::loadSimBot(Script::Block& block, Sim::BotD *simBot)
	{
		simBot->baseSpeed = ExtData::readValue<double>(
			block.getDataFirst("Speed"), 0);
		
		// Load debug data
		Sim::Collision::ColPoints pts;
		pts.push_back(Sim::Vector(0,0));
		pts.push_back(Sim::Vector(0,1));
		pts.push_back(Sim::Vector(1,1));
		pts.push_back(Sim::Vector(1,0));
		
		simBot->setCollision(new Sim::Collision(pts));
	}
	
	// ExtBot
	//
	//
	ExtBot::ExtBot() {}
	ExtBot::~ExtBot() {}
	
	void ExtBot::loadBlock(Script::Block& block, TypeRule* rule)
	{
		// Load standard data
		ExtBaseDataObj::loadBlock(block, rule);
		
		// Load extended data
		{
			mBaseCost = ExtData::readValue<uint32_t>(
				block.getDataFirst("BaseCost"), 0);
			
			// Load weapon slots
			Script::Block *slotBlock = block.getBlock("WEAPONS");
			for(size_t i=0; i<slotBlock->getSize(); i++) {
				Script::Data *slotData = slotBlock->getDataIndex(i);
				
				if(slotData->getId() == "Slot") {
					const std::string &slotType = slotData->getArg(0);
					int slotCount = ExtData::readValue<int>(slotData->getArg(1), 1);
					
					for(int sc=0; sc<slotCount; sc++)
						mWeaponSlot.push_back( WeaponSlot(slotType) );
				}
			}
		}
		
		// Load temporary data
		{
			// Load attachments
			mCoreHealth.loadData(block.getDataSimple("Health"));
			
			Script::Block *atmBlock = block.getBlock("ATTACHMENTS");
			if(atmBlock) {
				for(size_t hi=0; hi<atmBlock->getSize(); ++hi) {
					Script::Data *healthData = atmBlock->getDataIndex(hi);
					if(healthData) {
						mAttachmentHealth.push_back(HealthHull());
						mAttachmentHealth.back().loadData(*healthData);
					}
				}
			}
		}
	}
	
	void ExtBot::postProcess(ExtSim& extsim)
	{
		ExtBaseDataObj::postProcess(extsim);
		
		Sim::ArmorDatabase &armorDb = extsim.getSim().getData().getArmorDb();
		
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
	void ExtBot::HealthHull::loadData(Script::Data& data)
	{
		mType = data.getArg(0);
		
		int health, maxHealth;
		health = maxHealth = ExtData::readValue<int>(data.getArg(1), 1);
		if(data.argCount()==3)
			health = ExtData::readValue<int>(data.getArg(2), maxHealth);
		
		addMaxHealth(maxHealth, false);
		addHealth(health);
	}
	
	void ExtBot::HealthHull::postProcess(Sim::ArmorDatabase& armorDb)
	{
		setType(armorDb.getIdOf(mType));
	}
}
