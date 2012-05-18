#include "ExtBot.h"
#include "../ExtData.h"

#include "../../simulation/Simulation.h"
#include "../ExtSim.h"

#include "../typerule/bot/DefaultBot.h"

namespace ExtS {
	// ExtBotData
	//
	//
	ExtBotData::ExtBotData(ExtSim &esim): DefaultExtData<ExtBot>(esim)
	{
		// A blank string identifies the default type,
		// or the type selected if no "Base" parameter is defined
		// in a script block
		registerTypeRule("", new Bot::DefaultBot());
	}
	
	ExtBotData::~ExtBotData()
	{}
	
	// ExtBot
	//
	//
	ExtBot::ExtBot(ExtSim *esim) : ExtBaseDataObj(esim) {}
	ExtBot::~ExtBot() {}
	
	void ExtBot::loadNode(DaScript::Node& node,
		Sim::IdType simTypeId, TypeRule* rule)
	{
		// Load standard data
		ExtBaseDataObj::loadNode(node, simTypeId, rule);
		loadSimBot(node,
			mExtSim->getSim().getData().getBotDb().getType(getId()));
		
		// Load extended data
		{
			mBaseCost = ExtData::readValue<uint32_t>(
				node.getNodeFirst("BaseCost"), 0);
			
			// Load weapon slots
			DaScript::Node &slotNode = node.getNode("WEAPONS");
			
			for(size_t i=slotNode.indexOf("Slot"); i!=DaScript::Node::NoPos;
			i=slotNode.nextIndexOf("Slot",i)) {
				DaScript::Node &data = slotNode.getNode(i);
				
				const std::string &slotType = data.getArg(0);
				int slotCount = ExtData::readValue<int>(data.getArg(1), 1);
				
				for(int sc=0; sc<slotCount; sc++)
					mWeaponSlot.push_back( WeaponSlot(slotType) );
			}
		}
		
		// Load temporary data
		{
			// Load attachments
			mCoreHealth.loadData(node.getNodeSimple("Health"));
			
			DaScript::Node &atmNode = node.getNode("ATTACHMENTS");
			if(atmNode.isValid()) {
				for(size_t hi=0, sz=atmNode.getNodeCount(); hi<sz; ++hi) {
					DaScript::Node &healthNode = atmNode.getNode(hi);
					mAttachmentHealth.push_back(HealthHull());
					mAttachmentHealth.back().loadData(healthNode);
				}
			}
		}
	}
	
	void ExtBot::postProcess(ExtSim& extsim)
	{
		ExtBaseDataObj::postProcess(extsim);
		
		Sim::ArmorDatabase &armorDb = extsim.getSim().getData().getArmorDb();
		mCoreHealth.postProcess(armorDb);
		
		Sim::BotD *simData = extsim.getSim().getData().getBotDb().
			getType(getId());
		
		simData->coreHealth.getCore() = mCoreHealth;
		
		for(AttachmentVec::iterator i=mAttachmentHealth.begin();
			i!=mAttachmentHealth.end(); ++i) {
			(*i).postProcess(armorDb);
			
			simData->coreHealth.addAttachment(*i);
		}
	}
	
	void ExtBot::loadSimBot(DaScript::Node& node, Sim::BotD *simBot)
	{
		simBot->baseSpeed = ExtData::readValue<double>(
			node.getNodeFirst("Speed"), 0);
		
		// Load debug data
		Sim::Collision::ColPoints pts;
		pts.push_back(Sim::Vector(0,0));
		pts.push_back(Sim::Vector(0,1));
		pts.push_back(Sim::Vector(1,1));
		pts.push_back(Sim::Vector(1,0));
		
		simBot->setCollision(new Sim::Collision(pts));
	}

	// ExtBot::HealthHull
	//
	//
	void ExtBot::HealthHull::loadData(DaScript::Node& data)
	{
		mType = data.getArg(0);
		
		int health, maxHealth;
		health = maxHealth = ExtData::readValue<int>(data.getArg(1), 1);
		if(data.getNodeCount()==3)
			health = ExtData::readValue<int>(data.getArg(2), maxHealth);
		
		addMaxHealth(maxHealth, false);
		addHealth(health);
	}
	
	void ExtBot::HealthHull::postProcess(Sim::ArmorDatabase& armorDb)
	{
		setType(armorDb.getIdOf(mType));
	}
}
