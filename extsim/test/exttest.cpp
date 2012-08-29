#include <stdio.h>
#include <stdint.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../ExtSim.h"

#include "../param/IdList.h"
#include "../param/Position.h"
#include "../param/ValRange.h"

#include "../object/TypeRule.h"

exts::ExtSim extSim = exts::ExtSim();
Sim::Simulation &sim = extSim.getSim();

namespace fs = boost::filesystem;

template<class T>
struct IdListListener : public exts::Listener<exts::IdList<T> > {
	void process(exts::IdList<T> *p) {
		std::cout << p->getId() << " | ";
		
		const typename exts::IdList<T>::IdSet &idSet = p->getIdSet();
		for(typename exts::IdList<T>::IdSet::const_iterator i=idSet.begin();
			i!=idSet.end(); ++i) {
			std::cout << *i << " ";
		}
		std::cout << (p->isConstraintDefined()?"":"(identity)");
	}
};

template<class T>
struct ValRangeListener : public exts::Listener<exts::ValRange<T> > {
	void process(exts::ValRange<T> *p) {
		std::cout << p->getVal();
	}
};

struct PositionListener : public exts::Listener<exts::PositionParam> {
	void process(exts::PositionParam *p) {
		p->setVal(Sim::Vector(5,25));
		
		std::cout << p->getVal().x << "," << p->getVal().y << "\n";
		
		const exts::PositionParam::ValPairVec &consts = p->getValPairs();
		for(exts::PositionParam::ValPairVec::const_iterator i = consts.begin();
			i!=consts.end(); ++i) {
			std::cout << "\t" << i->first.x << "," << i->first.y <<
				" -> " << i->second.x << "," << i->second.y << "\n";
		}
	}
};

void loadFiles()
{
	fs::directory_iterator endIter;
	for(fs::directory_iterator i( DATADIR ); i!=endIter; ++i) {
		if( fs::is_regular_file(i->status()) ) {
			printf("Loading script %s\n", i->path().string().c_str());
			
			std::ifstream ifs(i->path().string().c_str());
			std::string str;
			
			ifs.seekg(0, std::ios::end);
			str.reserve(ifs.tellg());
			ifs.seekg(0, std::ios::beg);
			
			str.assign((std::istreambuf_iterator<char>(ifs)),
				std::istreambuf_iterator<char>() );
			
			extSim.getData().loadScript( str );
		}
	}
}

void printDamageRule(const Sim::ArmorD::DamageRule &rule)
{
	printf("(mMultiplier=%g, mIsIgnoring=%s)\n",
		rule.mMultiplier, rule.mIsIgnoring?"true":"false");
}

void listArmor()
{
	Sim::ArmorDatabase &armorDb = sim.getData().getArmorDb();
	Sim::DamageDatabase &damageDb = sim.getData().getDamageDb();
	
	printf("Armor type list:\n");
	for(unsigned int i=0; i<armorDb.size(); i++) {
		printf("\t%s:\n", armorDb.getNameOf(i).c_str());
		
		const Sim::ArmorD *data = armorDb.getArmor(i);
		const Sim::ArmorD::RuleMap &rules = data->getRules();
		const Sim::ArmorD::DamageRule &def = data->getDefault();
		
		printf("\t\t%-16s", "Default:");
		printDamageRule(def);
		
		for(Sim::ArmorD::RuleMap::const_iterator j=
			rules.begin(); j!=rules.end(); j++) {
			printf("\t\t%-16s", ("("+damageDb.getNameOf(j->first)+"):").c_str());
			printDamageRule(j->second);
		}
	}
}

void listDamage()
{
	Sim::DamageDatabase &damageDb = sim.getData().getDamageDb();
	
	printf("Damage type list:\n");
	for(unsigned int i=0; i<damageDb.size(); i++) {
		printf("\t%s\n", damageDb.getNameOf(i).c_str());
	}
}

void printHull(const::Sim::Health::Hull &hull)
{
	printf("\t\t\tType: %s, health: %d / %d\n",
		sim.getData().getArmorDb().getNameOf(hull.getType()).c_str(),
		hull.getHealth(), hull.getMaxHealth() );
}

void printHealth(const Sim::Health &health)
{
	printf("\tHealth:\n");
	
	printf("\t\tCore:\n");
	printHull(health.getCore());
	printf("\t\tAttatchments:\n");
	for(Sim::Health::AttachmentVec::const_iterator i=
		health.getAttachments().begin(); i!=health.getAttachments().end(); ++i){
		printHull(*i);
	}
}

void printExtBot(const exts::ExtBot *bot)
{
	printf("\tName: %s\n", bot->getName().c_str());
	printf("\tDescription: %s\n", bot->getDescription().c_str());
}

void printSimBot(const Sim::BotD *bot)
{
	printHealth(bot->coreHealth);
}

void listBot()
{
	Sim::BotDatabase &simBotDb = sim.getData().getBotDb();
	exts::ExtBotData &extBotDb = extSim.getData().getBotDb();
	
	printf("Bot type list:\n");
	for(unsigned int i=0; i<simBotDb.size(); i++) {
		const Sim::BotD *simBot = static_cast<const Sim::BotD*>(simBotDb.getType(i));
		const exts::ExtBot *extBot = extBotDb.getType(i);
		
		printExtBot(extBot);
		printSimBot(simBot);
		
		std::cout << "\n";
	}
}

void testParam()
{
	exts::IdList<Sim::ArmorD>::setListener( IdListListener<Sim::ArmorD>() );
	exts::ValRange<uint32_t>::setListener( ValRangeListener<uint32_t>() );
	exts::PositionParam::ListenerSlot<exts::PositionParam>::setListener( PositionListener() );
	
	printf("\nParameter testing:\n");
	
	exts::ExtProgramData &progDb = extSim.getData().getProgramDb();
	for(Sim::IdType i=0; i<progDb.size(); ++i) {
		const exts::ExtProgram *prog = progDb.getType(i);
		std::cout << prog->getName() << "\n\tDescription: " <<
			prog->getDescription() << "\n";
		
		const exts::TypeRule *rule;
		if(prog && (rule=prog->getRule())!=0) {
			exts::ParamList *param = rule->makeParam();
			exts::ParamList::RuleParamVec &paramVec =
				param->getParamVec();
			
			for(exts::ParamList::RuleParamVec::iterator i=paramVec.begin();
			i!=paramVec.end(); ++i) {
				std::cout << "\t\"" << (*i)->getDataName() << "\"\t ";
				(*i)->callback();
				std::cout << "\n";
			}
			
			delete param;
		}
	}
}

void setupWorld()
{
	extSim.getAgent().setupAgents(2);
	
	Sim::Player testSide;
	testSide.mAllyGroup = 0;
	sim.getState().getPlayerData().addPlayer(testSide);
	
	Sim::Bot::Config botCfg;
	botCfg.mSide = 0;
	botCfg.mBody.mPos = Sim::Vector(0,0);
	
	sim.getInput().getBotInput().buildInputImpl<Sim::Bot>( botCfg, 0 );
}

void testSim()
{
	std::cout << "\nTesting ExtSim-Simulation communication\n";
	
	std::cout << "Giving bot input through ExtSim\n";
	
	// Create input object
	exts::ParamList *inputParam = extSim.getData().getProgramDb().getType(
		"MoveTowards"
	)->getRule()->makeParam(0);
	
	// Modify input object
	inputParam->traverseCallback();
	
	// Register input
	extSim.getInput().registerInput(inputParam);
	extSim.getCpuInput().registerCpuInput(0,inputParam->getAllocId(0),0);
	
	// Dispatching input
	extSim.getInput().dispatchInput();
	
	// Running a single phase to assure things work
	Sim::Vector pos = sim.getState().getBotFactory().getBot(0)->getBody().mPos;
	std::cout << "Bot position pre: ("<<pos.x<<", "<<pos.y<<")\n";
	
	sim.startPhase();
	while(sim.hasPhaseStep())
		sim.step();
	sim.endPhase();
	
	pos = sim.getState().getBotFactory().getBot(0)->getBody().mPos;
	std::cout << "Bot position post: ("<<pos.x<<", "<<pos.y<<")\n";
	
	std::cout << "\n";
}

/*
ExtSim interaction pseudocode:

[bootstrap]
ExtSim::startup();

[data loading]
for all data files as F:
	ExtSim::loadDataScript(F);
ExtSim::postProcessData();

[preparing the simulation]
create players
create bots
ExtSim::prepareSim();

[main loop]
while game running
	do any of the following
		- give input to extsim directly
		- build input using the InputBuilder
		- feed input from external packets
		- recover input from the ReplayManager or alternate timelines
		- select the active phase on the timeline, as well as the active timeline
		- revert to the "present" simulation state on the timeline
		- seek on the timeline or replay part of the timeline
	
	- ExtSim::finishPhase()
		stores the active input buffer in the ReplayManager
			all timeline data after the solidified phase is invalidated and discarded
		(optional) stores the active InputBuilder data in the ReplayManager

[shutting down the simulation]
(optional) save the ReplayManager data
ExtSim::shutdown();

Subprocesses:
[give input to ExtSim]
choose a target to create input for
	- for example program input for a specific bot
	- or the creation of a bot
ExtSim builds a ParamList for this object or operation
the ParamList object is modified by the caller as desired
the caller passes the ParamList back to ExtSim, either storing it in
	->InputBuilder, or passing it directly to the barrier

[feed input from external packets]
the caller receives external input packets as Save objects or similar
these objects are sent directly to either InputBuilder or to the barrier

[recover input from the ReplayManager or alternate timelines]
the caller contacts the ReplayManager and switches the active branch and node


 */

int main(void)
{
	// [bootstrap]
	extSim.startup();
	
	// [data loading]
	loadFiles();
	extSim.postProcessData();
	
	// [prepare simulation]
	setupWorld();
	extSim.prepareSim();
	
	printf("\n");
	listArmor();
	printf("\n");
	listDamage();
	printf("\n");
	listBot();
	
	// Testing parameters
	testParam();
	
	// Testing extsim-simulation communication
	testSim();
	
	return 0;
}
