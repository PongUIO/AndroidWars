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

ExtS::ExtSim extSim = ExtS::ExtSim();
Sim::Simulation &sim = extSim.getSim();

namespace fs = boost::filesystem;

template<class T>
struct IdListListener : public ExtS::Listener<ExtS::IdList<T> > {
	void process(ExtS::IdList<T> *p) {
		std::cout << p->getId() << " | ";
		
		const typename ExtS::IdList<T>::IdSet &idSet = p->getIdSet();
		for(typename ExtS::IdList<T>::IdSet::const_iterator i=idSet.begin();
			i!=idSet.end(); ++i) {
			std::cout << *i << " ";
		}
		std::cout << (p->isAlwaysValid()?"(all)":"");
		std::cout << (p->isConstraintUndefined()?"(identity)":"");
	}
};

template<class T>
struct ValRangeListener : public ExtS::Listener<ExtS::ValRange<T> > {
	void process(ExtS::ValRange<T> *p) {
		std::cout << p->getVal();
	}
};

struct PositionListener : public ExtS::Listener<ExtS::PositionParam> {
	void process(ExtS::PositionParam *p) {
		p->setVal(Sim::Vector(25,25));
		
		std::cout << p->getVal().x << "," << p->getVal().y << "\n";
		
		const ExtS::PositionParam::ValPairVec &consts = p->getValPairs();
		for(ExtS::PositionParam::ValPairVec::const_iterator i = consts.begin();
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
			printf("Loading script %s\n", i->string().c_str());
			
			std::ifstream ifs(i->string().c_str());
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

void printExtBot(const ExtS::ExtBot *bot)
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
	ExtS::ExtBotData &extBotDb = extSim.getData().getBotDb();
	
	printf("Bot type list:\n");
	for(unsigned int i=0; i<simBotDb.size(); i++) {
		const Sim::BotD *simBot = simBotDb.getDataById(i);
		const ExtS::ExtBot *extBot = extBotDb.getDataById(i);
		
		printExtBot(extBot);
		printSimBot(simBot);
	}
}

void testParam()
{
	ExtS::IdList<Sim::ArmorD>::setListener( IdListListener<Sim::ArmorD>() );
	ExtS::ValRange<uint32_t>::setListener( ValRangeListener<uint32_t>() );
	ExtS::PositionParam::ListenerSlot<ExtS::PositionParam>::setListener( PositionListener() );
	
	printf("\nParameter testing:\n");
	
	ExtS::ExtProgramData &progDb = extSim.getData().getProgramDb();
	for(Sim::IdType i=0; i<progDb.size(); ++i) {
		const ExtS::ExtProgram *prog = progDb.getDataById(i);
		std::cout << prog->getName() << "\n\tDescription: " <<
			prog->getDescription() << "\n";
		
		const ExtS::TypeRule *rule;
		if(prog && (rule=prog->getRule())!=0) {
			ExtS::ParamList *param = rule->makeParam();
			const ExtS::TypeRule::RuleParamVec &paramVec =
				param->getRuleParamVec();
			
			for(ExtS::TypeRule::RuleParamVec::const_iterator i=paramVec.begin();
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
	Sim::Player testSide;
	testSide.mAllyGroup = 0;
	sim.getState().getPlayerData().addPlayer(testSide);
	
	Sim::Bot::Config botCfg;
	botCfg.mSide = 0;
	botCfg.mType = 0;
	botCfg.mBody.mPos = Sim::Vector(0,0);
	
	sim.getInput().getBotInput().buildInput( botCfg );
}

void testSim()
{
	std::cout << "\nTesting ExtSim-Simulation communication\n";
	
	std::cout << "Giving bot input through ExtSim\n";
	
	// Create input object
	ExtS::InputData inputData = extSim.getInput().getProgram().buildInput(
		extSim.getData().getProgramDb().getIdOf("MoveTowards")
	);
	
	// Modify input object
	ExtS::InputData::ParamPtr paramPtr = inputData.getParamList();
	const ExtS::TypeRule::RuleParamVec paramVec = paramPtr->getRuleParamVec();
	for(uint32_t i=0; i<paramVec.size(); ++i) {
		paramVec[i]->callback();
	}
	
	// Register input
	extSim.getInput().getProgram().registerInput(inputData);
	
	// Dispatching input
	extSim.getInput().getProgram().dispatchInput(ExtS::IcmNone);
	
	// (Temporary to bypass a lack of cpu input in extsim)
	sim.getInput().getCpuInput().registerInput(0,0,0);
	
	// Running a single phase to assure things work
	Sim::Vector pos = sim.getState().getBotFactory().getBot(0)->getBody().mPos;
	std::cout << "Bot position pre: ("<<pos.x<<", "<<pos.y<<")\n";
	
	sim.startPhase();
	while(sim.hasPhaseStep())
		sim.step();
	sim.endPhase(true);
	
	pos = sim.getState().getBotFactory().getBot(0)->getBody().mPos;
	std::cout << "Bot position post: ("<<pos.x<<", "<<pos.y<<")\n";
	
	std::cout << "\n";
}

int main(void)
{
	Sim::Configuration config;
	config.phaseLength = 5;
	config.stepTime = 0.01;
	
	// [bootstrap]
	extSim.startup();
	extSim.getSim().getData().getProgramDb().registerAllDefault();
	extSim.getSim().getData().getAbilityDb().registerAllDefault();
	
	// [data loading]
	extSim.switchDataContext(ExtS::ExtData::LcDataLoading);
	loadFiles();
	extSim.postProcessData();
	
	// [content loading]
	extSim.switchDataContext(ExtS::ExtData::LcContentLoading);
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
