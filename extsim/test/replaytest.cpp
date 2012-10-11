#include <stdio.h>
#include <boost/filesystem.hpp>
#include <fstream>

#include "../ExtSim.h"

#include "../param/IdParam.h"
#include "../param/Position.h"
#include "../param/ValueParam.h"

#include "../object/TypeRule.h"

exts::ExtSim extSim = exts::ExtSim();
Sim::Simulation &sim = extSim.getSim();

namespace fs = boost::filesystem;

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

void printErrors()
{
	const exts::ExtErrorMgr::MessageVec &msgVec = extSim.getError().getMessages();
	if(msgVec.size() > 0)
		printf("ExtSim error list:\n");
	
	for(size_t i=0; i<msgVec.size(); ++i) {
		exts::ExtErrorMgr::Message msg = msgVec.at(i);
		printf("\t%s : %s\n", msg.where.c_str(), msg.what.c_str());
	}
}

void setupWorld()
{
	extSim.getAgent().setupAgents(2);
	
	Sim::Player testSide;
	testSide.mAllyGroup = 0;
	sim.getState().getPlayerData().addPlayer(testSide);
	
	Sim::Bot::Config botA;
	botA.mSide = 0;
	botA.mBody.mPos = Sim::Vector(0,0);
	
	sim.getState().getBotFactory().createImpl<Sim::Bot>( botA, 0, 0 );
	sim.getState().getBotFactory().createImpl<Sim::Bot>( botA, 0, 1 );
}

void buildBotInput(uint32_t depth)
{
	if(depth == 3)
		return;
	else {
		const exts::ReplayNode *activeNode = extSim.getReplay().getActiveNode();
		Sim::IdType activeId = activeNode->getId();
		
		extSim.getReplay().selectBranch(activeId);
		extSim.getReplay().gotoActive();
		
		// Create the branches
		for(size_t i=0; i<2; ++i) {
			extSim.getReplay().selectBranch(activeId);
			extSim.getReplay().gotoActive();
			
			exts::ParamList *paramList = extSim.getData().getProgramDb()
				.getType("MoveTowards")->getRule()->makeParam(0);
			
			double modVal = (i==0 ? 1.0 : -1.0);
			Sim::Vector offPos = Sim::Vector(activeId,1+activeNode->getDepth()*modVal);
			
			paramList->getParamT<exts::PositionParam>(0)->setVal(offPos);
			
			extSim.getInput().registerInput(paramList);
			extSim.getCpuInput().registerCpuInput(0,
				paramList->getAllocId(0), 0);
			
			extSim.getInput().dispatchInput();
			
			buildBotInput(depth+1);
		}
	}
}

/*
 * Generates a branching replay where one bot will select one of two possible
 * paths recursively.
 */
void generateReplay()
{
	buildBotInput(0);
}

void displayNode(const exts::ReplayNode *node)
{
	const exts::ReplayNodeVec &nodeVec = node->getChildren();
	
	extSim.getReplay().selectBranch(node->getId());
	extSim.getReplay().gotoActive();
	printf("%s%d (%X): ", std::string(node->getDepth(),'\t').c_str(),
		node->getId(), sim.checksumSim());
	Sim::Vector botPos = sim.getState().getBotFactory().getBot(0)->getBody().mPos;
	printf("(%d) (%g, %g) ", sim.getCurPhase(), botPos.x, botPos.y);
	
	if(node->getChildrenCount() > 0) {
		printf("has children = (");
		
		for(exts::ReplayNodeVec::const_iterator i=nodeVec.begin();
		i!=nodeVec.end(); ++i) {
			printf("%d, ", (*i)->getId());
		}
		printf(")");
	}
	printf("\n");
	
	for(exts::ReplayNodeVec::const_iterator i=nodeVec.begin();
	i!=nodeVec.end(); ++i) {
		displayNode(*i);
	}
}

void displayReplayTree()
{
	const exts::ReplayNode *node = extSim.getReplay().getReplayTree().getRoot();
	
	displayNode(node);
}

int main(void)
{
	// Setup data
	extSim.startup();
	loadFiles();
	extSim.postProcessData();
	
	// Setup world
	setupWorld();
	extSim.prepareSim();
	generateReplay();
	
	// Analyze replay
	displayReplayTree();
	
	// Display any errors
	printErrors();
	
	// Shutdown the simulation
	extSim.shutdown();
	
	return 0;
}
