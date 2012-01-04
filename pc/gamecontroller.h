#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include<QtGui>
#include<stdlib.h>
#include<typeinfo>

#include "Simulation.h"
#include "../openglqt/render.h"
#include "../util/client.h"
#include "botselector.h"
#include "gamebutton.h"

#define RATE 20

class GameController {
public:
	MainWidget *parent;
	GameDrawer *drawer;
	QTimer *glTimer, *camTimer, *timer;
	QHBoxLayout *lower, *upper;
	QVBoxLayout *iconHolder;
	QSpacerItem *space;
	QPixmap profiles[1];
	GameButton *label, *label2;
	Sim::Simulation sim;
	QPixmap m;
	GameController(MainWidget *parent = 0) {
		this->parent = parent;

		//init sim - note: currently test only.
		Sim::Configuration config;

		config.phaseLength = 10;
		config.stepTime = 0.01;
		sim.startup(config);
		
		sim.getData().getProgramDb().registerAllDefault();

		Sim::Player testSide;
		sim.getState().getPlayerData().addPlayer(testSide);

		Sim::BotD myBot;
		myBot.baseSpeed = 100.0;
		myBot.baseWeight = 75.0;
		myBot.cpuCycleSpeed = 100;
		myBot.cpuStorage = 100;

		Sim::Collision::ColPoints cpts;
		cpts.push_back(Sim::Vector(0,0));
		cpts.push_back(Sim::Vector(0,1.8));
		cpts.push_back(Sim::Vector(1.3,1.8));
		cpts.push_back(Sim::Vector(1.3,0));

		sim.getData().getBotDb().addBot(myBot, cpts);

		// Create a test bot
		Sim::Bot::Config botCfg;
		botCfg.mSide = 0;
		botCfg.mType = 0;
		botCfg.mBody.mPos = Sim::Vector(0,0);
		uint32_t botId = sim.getState().getBotFactory().createBot( botCfg );
		botCfg.mBody.mPos = Sim::Vector(0,1);
		sim.getState().getBotFactory().createBot( botCfg );

		// Send some input to this bot
		{
			Sim::InputManager &inMgr = sim.getState().getInputManager();
			Sim::ProgramFactory &progFact = sim.getState().getProgramFactory();
			
			using namespace Sim::Prog;
			MoveTowards *move = progFact.createProgram<MoveTowards>(
				MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(5,0))
			);
			
			Kill *kill = progFact.createProgram<Kill>(
				Kill::Config(move->getId()));
			
			inMgr.registerInput(botId, move->getId(), 0);
			inMgr.registerInput(botId, kill->getId(), 20);
		}
		
		Sim::TileDatabase &db = sim.getData().getTileDb();
		Sim::TileD myTile;
		myTile.colMask = 0;
		myTile.blastResist = 0.0;
		myTile.bounce = 0.0;
		myTile.friction = 0.0;
		db.addTile(myTile);
		myTile.colMask = Sim::TileD::ColAll;
		db.addTile(myTile);
		myTile.colMask = Sim::TileD::ColAll;
		db.addTile(myTile);

		sim.getState().getWorld().getTile(3,0).setType(1);

		sim.prepareSim();
		//end of sim init

		parent->states->setSim(&sim);

		drawer = new GameDrawer(parent);

		glTimer = new QTimer(parent);
		drawer->connect(glTimer, SIGNAL(timeout()), drawer, SLOT(redraw()));
		glTimer->start(0);

		camTimer = new QTimer(parent);
		drawer->connect(camTimer, SIGNAL(timeout()), drawer, SLOT(moveMouseCheck()));
		camTimer->start(1000./RATE);

		iconHolder  = new QVBoxLayout(parent);
		lower = new QHBoxLayout();
		upper = new QHBoxLayout();
		iconHolder->insertLayout(0, lower);
		iconHolder->insertSpacing(1, -1);
		iconHolder->insertLayout(0, upper);
		parent->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
		drawer->setMouseTracking(true);

		label = new GameButton(&sim, 0, parent);
		label2 = new GameButton(&sim, 0, parent);
		label->setPixmap(QPixmap(":/graphics/temp/temp.png"));
		label->resize(60,60);
		label2->setPixmap(QPixmap(":/graphics/temp/temp2.png"));
		label2->resize(60,60);
		profiles[0] = QPixmap(":/graphics/profiles/test.png");
		space = new QSpacerItem(parent->width(), 0, QSizePolicy::Expanding);
		label->show();
		label2->show();
		lower->insertWidget(0, label);
		lower->insertWidget(1, label2);
		lower->insertSpacerItem(2, space);
		lower->setAlignment(Qt::AlignBottom);
		updateGUI();
		botCfg.mBody.mPos = Sim::Vector(0,2);
		sim.getState().getBotFactory().createBot( botCfg );
		upper->setAlignment(Qt::AlignTop);
    }
	void showAll() {
		parent->show();
		label->show();
		label2->show();
		drawer->show();
		updateGUI();
	}
	void hideAll() {
		parent->show();
		label->hide();
		label2->hide();
		drawer->hide();
		emptyGUI();
	}

	void updateGUI() {
		emptyGUI();
		int i;
		std::list<Sim::Bot*> bots = sim.getState().getBotFactory().getBotList();
		std::list<Sim::Bot*>::iterator it;
		BotSelector *clabel;
		for (it = bots.begin(); it != bots.end(); it++) {
			clabel = new BotSelector((*it)->getId(), parent->states, parent);
			clabel->setPixmap(profiles[0]);
			clabel->resize(60,60);
			upper->insertWidget(i, clabel);
			clabel->show();
		}
		upper->insertSpacerItem(i, space);
	}

	void emptyGUI() {
		QLayoutItem *temp;
		while ((temp = upper->takeAt(0))) {
			delete temp->widget();
		}
	}
	void changeBot(int bot) {
		updateGUI();
	}
};

#endif // GAMECONTROLLER_H
