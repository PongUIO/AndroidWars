#ifndef MAIN_H
#define MAIN_H

#include<QtGui>
#include<QWidget>
#include<QDebug>
#include<vector>
#include"../util/client.h"
#include"gamecontroller.h"
#include"menubutton.h"

class MainWidget : public QWidget {
Q_OBJECT
public slots:
	void receiveClick(int type, int value) {
		if (type == 1) {
			changeMenu(value);
		} else {
			setMenu(value);
		}
	}
public:
	QPalette *p;
	bool fullScreen, menu, gameRunning;
	ClientStates *states;
	GameController *gc;
	Sim::Simulation sim;
	MenuButton *gameButton;
	MainWidget (QWidget *parent = 0) : QWidget(parent) {
		grabKeyboard();
		states = new ClientStates();
		initSim();
		fullScreen = false;
		p = new QPalette( this->palette() );
		p->setColor( QPalette::Background, Qt::black );
		setPalette( *p );
		menu = true;
		gameRunning = false;
		gc = new GameController(states, this);
		gc->hideAll();
		registerForResize(gc->drawer);
		resize(700,700);
		initMenus();
	}
	void registerForResize(QWidget *target) {
		res.push_back(target);
	}
	void setFullScreen(bool state) {
		if (state) {
			showFullScreen();
		} else {
			showNormal();
		}
	}
	void setStates(ClientStates *nS) {
		states = nS;
		states->releaseMods();
	}
	void changeMenu(int menu) {

	}

	void toggleMenu() {
		setMenu(!menu);
	}
	void setMenu(bool state) {
		menu = state;
		if (state) {
			gc->hideAll();
			showMenus();
		} else {
			gc->showAll();
			hideMenus();
		}
	}
	void hideMenus() {
		gameButton->hide();
	}
	void showMenus() {
		moveMenuElements();
		gameButton->show();
	}
	void setMouse() {
		QPixmap m;
		m.convertFromImage(QImage(":/graphics/mouse/default.png").scaled(64,64));
		setCursor(m);
	}
	void initMenus() {
		gameButton = new MenuButton(0, 0, this);
		gameButton->setPixmap(QPixmap(":/graphics/menu/startgame.png"));
		gameButton->resize(gameButton->pixmap()->size());
		gameButton->show();
		connect(gameButton, SIGNAL(onClick(int, int)), this, SLOT(receiveClick(int, int)));
		moveMenuElements();
	}
	void moveMenuElements() {
		gameButton->move(geometry().width()/2-gameButton->pixmap()->width()/2, geometry().height()/2-gameButton->pixmap()->height()/2);
	}
	void initSim() {
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
		botCfg.mBody.mPos = Sim::Vector(0,2);
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

		states->setSim(&sim);
	}

protected:
	std::vector<QWidget*> res;
	// overridden
	void resizeEvent(QResizeEvent *event) {
		resize(event->size());
		if (menu) {
			moveMenuElements();
		}
		for (int i = 0; i < res.size(); i++) {
			#ifdef _WIN32
			res[i]->resize(event->size().width(), event->size().height()-1);
			#else
			res[i]->resize(event->size());
			#endif
		}
	}
	void keyPressEvent(QKeyEvent *event) {
		int k = event->key();
		if (k == Qt::Key_F11) {
			fullScreen = !fullScreen;
			setFullScreen(fullScreen);
		} else if (k == Qt::Key_Escape) {
			toggleMenu();
		} else {
			if (states != NULL) {
				modKey(k, true);
			}
		}
	}

	void keyReleaseEvent(QKeyEvent *event) {
		if (states != NULL) {
			modKey(event->key(), false);
		}
	}
	void modKey(int k, bool state) {
		if ( k == Qt::Key_Shift ) {
			states->setShift(state);
		} else if ( k == Qt::Key_Control ) {
			states->setCtrl(state);
			if (state) {

			}
		}
	}

};

#endif
