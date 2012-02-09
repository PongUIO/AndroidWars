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
struct IdListPListener : public ExtS::Listener<ExtS::IdListP<T> > {
	void process(ExtS::IdListP<T> *p) {
		std::cout << p->getId();
	}
};

template<class T>
struct IdListCListener : public ExtS::Listener<ExtS::IdListC<T> > {
	void process(ExtS::IdListC<T> *p) {
		const typename ExtS::IdListC<T>::IdSet &idSet = p->getIdSet();
		for(typename ExtS::IdListC<T>::IdSet::const_iterator i=idSet.begin();
			i!=idSet.end(); ++i) {
			std::cout << *i << " ";
		}
		std::cout << (p->isAlwaysValid()?"(all)":"");
	}
};

template<class T>
struct ValueRangeListener : public ExtS::Listener<ExtS::ValueRangeP<T> > {
	void process(ExtS::ValueRangeP<T> *p) {
		std::cout << p->getVal();
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
	ExtS::BotData &extBotDb = extSim.getData().getBotDb();
	
	printf("Bot type list:\n");
	for(unsigned int i=0; i<simBotDb.size(); i++) {
		const Sim::BotD *simBot = simBotDb.getType(i);
		const ExtS::ExtBot *extBot = extBotDb.getType(i);
		
		printExtBot(extBot);
		printSimBot(simBot);
	}
}

void testParam()
{
	ExtS::IdListP<Sim::ArmorD>::setListener( IdListPListener<Sim::ArmorD>() );
	ExtS::IdListC<Sim::ArmorD>::setListener( IdListCListener<Sim::ArmorD>() );
	ExtS::ValueRangeP<uint32_t>::setListener( ValueRangeListener<uint32_t>() );
	
	ExtS::ProgramData &progDb = extSim.getData().getProgramDb();
	for(Sim::IdType i=0; i<progDb.size(); ++i) {
		const ExtS::ExtProgram *prog = progDb.getType(i);
		std::cout << prog->getName() << "\n";
		
		const ExtS::TypeRule *rule;
		if(prog && (rule=prog->getRule())!=0) {
			ExtS::ParamList *param = rule->makeParam();
			const ExtS::TypeRule::MetaParamVec &paramVec =
				rule->getMetaParamVec();
			
			for(ExtS::TypeRule::MetaParamVec::const_iterator i=paramVec.begin();
				i!=paramVec.end(); ++i) {
				std::cout << "\t\"" << (*i)->getDataName() << "\"\t ";
				if( (*i)->getDefaultParam() )
					(*i)->getDefaultParam()->callback();
				std::cout << " | ";
				if( (*i)->getDefaultConstraint() )
					(*i)->getDefaultConstraint()->callback();
				std::cout << "\n";
			}
			
			delete param;
		}
	}
}

int main(void)
{
	Sim::Configuration config;
	config.phaseLength = 5;
	config.stepTime = 0.01;
	
	// [bootstrap]
	extSim.startup();
	
	// [data loading]
	extSim.switchDataContext(ExtS::ExtData::LcDataLoading);
	loadFiles();
	extSim.postProcessData();
	
	// [content loading]
	extSim.switchDataContext(ExtS::ExtData::LcContentLoading);
	loadFiles();
	extSim.postProcessData();
	
	// [prepare simulation]
	extSim.prepareSim();
	
	printf("\n");
	listArmor();
	printf("\n");
	listDamage();
	printf("\n");
	listBot();
	
	// Testing parameters
	testParam();
	
	return 0;
}
