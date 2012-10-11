#include "ExtBot.h"

#include "../ExtData.h"
#include "../util/TypeConv.h"

#include "../../simulation/Simulation.h"
#include "../ExtSim.h"

#include "../object/TypeRule.h"

#include "../typerule/bot/bot.h"

namespace exts {
	// ExtBotData
	//
	//
	ExtBotData::ExtBotData(ExtSim &esim) :
		DefaultExtData<ExtBot>(esim),
		mRuleLoader(esim)
	{
#define _EXTS_X(name, cls) mRuleLoader.registerRule(name, new bot::cls(mExtSim));
		#include "../typerule/bot/bot.def"
#undef _EXTS_X
	}
	
	ExtBotData::~ExtBotData()
	{}
	
	// ExtBot
	//
	//
	ExtBot::ExtBot(ExtSim &esim, Sim::IdType id) : ExtDataObjBase(esim,id) {}
	ExtBot::~ExtBot() {}
	
	void ExtBot::loadNode(const Nepeta::Node& node)
	{
		// Create simulation type
		mRule = mExtSim.getData().getBotDb().mRuleLoader.loadRuleNode(node);
		if(mRule) {
			mRule->setObjectId(getId());
			mRule->load(node);
			Sim::IdType simId = mRule->registerSimData(getName());
			
			assert(simId == getId() &&
			"Simulation ID must be the same as the ExtSim ID");
		}
		
		// Load standard data
		ExtDataObjBase::loadNode(node);
		loadSimBot(node,
			mExtSim.getSim().getData().getBotDb().getType(getId())
		);
		
		// Load extended data
		{
			mBaseCost = convValue<uint32_t>(node.getNodeFirst("BaseCost"), 0);
			
			// Load weapon slots
			const Nepeta::Node &slotNode = node.getNode("WEAPONS");
			
			for(Nepeta::Iterator i=slotNode.begin("Slot"); slotNode.hasNext(i);
			slotNode.next(i,"Slot")) {
				const Nepeta::Node &data = slotNode.getIterNode(i);
				
				const std::string &slotType = data.getArg(0);
				int slotCount = convValue<int>(data.getArg(1), 1);
				
				for(int sc=0; sc<slotCount; sc++)
					mWeaponSlot.push_back( WeaponSlot(slotType) );
			}
		}
		
		// Load temporary data
		{
			// Load attachments
			mCoreHealth.loadData(node.getNode("Health"));
			
			const Nepeta::Node &atmNode = node.getNode("ATTACHMENTS");
			if(atmNode.isValid()) {
				for(Nepeta::Iterator i=atmNode.begin(); atmNode.hasNext(i);
				atmNode.next(i)) {
					const Nepeta::Node &healthNode = atmNode.getIterNode(i);
					mAttachmentHealth.push_back(HealthHull());
					mAttachmentHealth.back().loadData(healthNode);
				}
			}
		}
	}
	
	void ExtBot::postProcess()
	{
		ExtDataObjBase::postProcess();
		
		Sim::ArmorDatabase &armorDb = mExtSim.getSim().getData().getArmorDb();
		mCoreHealth.postProcess(armorDb);
		
		Sim::BotD *simData = mExtSim.getSim().getData().getBotDb().
			getType(getId());
		
		simData->coreHealth.getCore() = mCoreHealth;
		
		for(AttachmentVec::iterator i=mAttachmentHealth.begin();
			i!=mAttachmentHealth.end(); ++i) {
			(*i).postProcess(armorDb);
			
			simData->coreHealth.addAttachment(*i);
		}
	}
	
	void ExtBot::loadSimBot(const Nepeta::Node& node, Sim::BotD *simBot)
	{
		simBot->baseSpeed = convValue<double>(node.getNodeFirst("Speed"), 0);
		
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
	void ExtBot::HealthHull::loadData(const Nepeta::Node& data)
	{
		mType = data.getArg(0);
		
		int health, maxHealth;
		health = maxHealth = convValue<int>(data.getArg(1), 1);
		if(data.getNodeCount()==3)
			health = convValue<int>(data.getArg(2), maxHealth);
		
		addMaxHealth(maxHealth, false);
		addHealth(health);
	}
	
	void ExtBot::HealthHull::postProcess(Sim::ArmorDatabase& armorDb)
	{
		setType(armorDb.getIdOf(mType));
	}
}
