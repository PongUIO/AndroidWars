#ifndef MAIN_H
#define MAIN_H

#include<QtGui>
#include<QWidget>
#include<QDebug>
#include<vector>
#include<stdlib.h>
#include "../util/client.h"
#include "gamecontroller.h"
#include "menubutton.h"
#include "menudefines.h"
#include "../extsim/ExtSim.h"

class MainWidget : public QWidget {
	Q_OBJECT
public slots:
	void receiveClick(State type, int value) {
		switch(type) {
		case GAME:
			setState(GAME);
			subMenu = RESUME;
			break;
		case MENU:
			changeToSubMenu(value);
			break;
		case EXIT:
			QApplication::quit();
			break;
		default:
			qDebug() << "What on earth is this state? " << type;
		}
	}
	void gameStep() {
		if (!states->getRunning()) {
			return;
		} else if ( sim.hasPhaseStep() ) {
			sim.step();
		} else {
			sim.endPhase(true);
			states->setRunning(false);
		}
	}
public:
	QPalette *p;
	bool fullScreen, gameRunning;
	State dispState;
	int subMenu;
	ClientStates *states;
	GameController *gc;
	ExtS::ExtSim eSim;
	Sim::Simulation &sim;
        QSize size;
	std::vector< std::vector<QWidget*> > menus;
	QTimer *gameTimer;
	double lastUpdate;

	MainWidget (QWidget *parent = 0) : QWidget(parent), sim(eSim.getSim()) {
		dispState = MENU;
		subMenu = MAIN;
		//grabKeyboard();
		states = new ClientStates();
		loadFiles("../data/");
		initSim();
		fullScreen = false;
		p = new QPalette( this->palette() );
		p->setColor( QPalette::Background, Qt::black );
		setPalette( *p );
		gameRunning = false;
                gc = new GameController(states, this);
                gc->hideAll();
                registerForResize(gc->drawer);
                resize(700,700);
		initMenus();
		gameTimer = new QTimer(this);
		connect(gameTimer, SIGNAL(timeout()), this, SLOT(gameStep()));
		resetTimer();
	}
	void loadFiles(QString path) {
		eSim.switchDataContext(ExtS::ExtData::ListenerContext(ExtS::ExtData::LcContentLoading |ExtS::ExtData::LcDataLoading));
		QDir dir(path);
		QStringList filters;
		filters << "*.da";
		QStringList files = dir.entryList(filters);
		for (QStringList::Iterator iter = files.begin(); iter != files.end(); ++iter) {
			QFile f(path + *iter);
			f.open(QFile::QIODevice::ReadOnly);
			eSim.loadDataScript(std::string(f.readAll()));
		}
		eSim.postProcessData();

	}
	void resetTimer() {
		gameTimer->stop();
		gameTimer->start(sim.getConfiguration().stepTime*1000);
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
	void changeToSubMenu(int value) {
		subMenu = value;
		showMenus();
	}
	void toggleMenu() {
		switch (dispState) {
		case GAME:
			setState(MENU);
			break;
		case MENU:
			setState(GAME);
			break;
		}
	}
	void setState(State s) {
		dispState = s;
		switch (s) {
		case MENU:
			gc->hideAll();
			showMenus();
			break;
		case GAME:
			gc->showAll();
			hideMenus();
			break;
		}
	}
	void hideMenus() {
		states->setMenu(false);
		for (int i = 0; i < menus.size(); i++) {
			for (int j = 0; j < menus[i].size(); j++) {
				menus[i][j]->hide();
			}
		}
	}
	void showMenus() {
		hideMenus();
		states->setMenu(true);
		if (subMenu < 0 || menus.size() < subMenu) {
			qDebug() << "Submenu-index out of range!!!";
		} else {
			for (int j = 0; j < menus[subMenu].size(); j++) {
				menus[subMenu][j]->show();
			}
		}
	}
	void addMenusToWidget() {
		for (int i = 0; i < menus.size(); i++) {
			for (int j = 0; j < menus[i].size(); j++) {
				layout()->addWidget(menus[i][j]);
			}
		}
	}
	void setToHidden() {

	}
	void setMouse() {
		QPixmap m;
		m.convertFromImage(QImage(":/graphics/mouse/default.png").scaled(64,64));
		setCursor(m);
	}
	void initMenus() {
		addMenuPage();
		addMenuButton(createButton(GAME, 0, QString("Start Game")));
		addMenuButton(createButton(EXIT, 0, QString("Exit")));
		addMenuPage();
		addMenuButton(createButton(GAME, 0, QString("Resume Game")));
		addMenuButton(createButton(MENU, MAIN, QString("To main menu")));
		addMenuButton(createButton(EXIT, 0, QString("Exit")));
		addMenusToWidget();
		showMenus();

	}

	void addMenuPage() {
		menus.push_back(std::vector<QWidget*>());
	}

	void addMenuButton(QWidget *in) {
		menus[menus.size()-1].push_back(in);
	}

	MenuButton *createButton(State type, int signal, QString text) {
		MenuButton *menuButton;
		menuButton = new MenuButton(type, signal, this);
		menuButton->setText(text);
		menuButton->hide();
		connect(menuButton, SIGNAL(onClick(State, int)), this, SLOT(receiveClick(State, int)));
		return menuButton;
	}

	void fixRes() {
		for (int i = 0; i < res.size(); i++) {
#ifdef _WIN32
			res[i]->resize(size.width(), size.height()-1);
#else
			res[i]->resize(size);
#endif
		}
	}
	void initSim() {
		Sim::Configuration &config = sim.getConfiguration();

		config.phaseLength = 10;
		config.stepTime = 0.01;
		sim.startup();

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

		//sim.getData().getBotDb().addBot(myBot, cpts);
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
			Sim::Input &inMgr = sim.getInput();
			Sim::ProgramFactory &progFact = sim.getState().getProgramFactory();

			using namespace Sim::Prog;
			Sim::IdType moveId = inMgr.getProgramInput().buildInputImpl<MoveTowards>(
						MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(10,0)))->
					getId();

			Kill *kill = inMgr.getProgramInput().buildInputImpl<Kill>(
						Kill::Config(moveId));
			inMgr.getCpuInput().registerInput(botId, moveId, 0);
			inMgr.getCpuInput().registerInput(botId, kill->getId(), 20);
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

		//sim.getState().getWorld().getTile(3,0).setType(1);

		sim.prepareSim();

		states->setSim(&sim);
	}

protected:
	std::vector<QWidget*> res;
	// overridden
	void resizeEvent(QResizeEvent *event) {
		size = event->size();
		//resize(size);
		fixRes();
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
