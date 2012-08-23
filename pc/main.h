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
			Submenu = RESUME;
			break;
		case MENU:
			changeToSubmenu(value);
			break;
		case EXIT:
			QApplication::quit();
			break;
		default:
			qDebug() << "What on earth is this state? " << type;
		}
	}
	void gameStep() {
		if (!States->getRunning()) {
			return;
		} else if ( SimFull.hasPhaseStep() ) {
			SimFull.step();
		} else {
			SimFull.endPhase(false);
			States->setRunning(false);
		}
	}
public:
	QPalette *pal;
	bool FullScreen, GameRunning;
	State DispState;
	int Submenu;
	ClientStates *States;
	GameController *GameCont;
	exts::ExtSim ExSim;
	Sim::Simulation &SimFull;
	QSize CurrentSize;
	std::vector< std::vector<QWidget*> > Menus;
	QTimer *GameTimer;
	double LastUpdate;
	QSettings *Settings;
	MainWidget (QWidget *parent = 0) : QWidget(parent), SimFull(ExSim.getSim()) {
		Settings = new QSettings("PongUIO", "AndroidWars");
		DispState = MENU;
		Submenu = MAIN;
		States = new ClientStates();
		loadFiles("../testmod/data/");
		initSim();
		FullScreen = false;
		pal = new QPalette( this->palette() );
		pal->setColor( QPalette::Background, Qt::black );
		setPalette( *pal );
		GameRunning = false;
		GameCont = new GameController(States, this);
		GameCont->hideAll();
		registerForResize(GameCont->drawer);
		resize(700,700);
		initMenus();
		GameTimer = new QTimer(this);
		connect(GameTimer, SIGNAL(timeout()), this, SLOT(gameStep()));
		resetTimer();
	}
	void loadFiles(QString path) {
		QDir dir(path);
		QStringList filters;
		filters << "*.nep";
		QStringList files = dir.entryList(filters);
		for (QStringList::Iterator iter = files.begin(); iter != files.end(); ++iter) {
			QFile f(path + *iter);
			f.open(QFile::QIODevice::ReadOnly);
			ExSim.loadDataScript(std::string(f.readAll()));
			f.close();
		}
		ExSim.postProcessData();

	}
	void resetTimer() {
		GameTimer->stop();
		GameTimer->start(SimFull.getConfiguration().stepTime*1000);
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
		States = nS;
		States->releaseMods();

	}
	void changeToSubmenu(int value) {
		Submenu = value;
		showMenus();
	}
	void toggleMenu() {
		switch (DispState) {
		case GAME:
			setState(MENU);
			break;
		case MENU:
			setState(GAME);
			break;
		}
	}
	void setState(State s) {
		DispState = s;
		switch (s) {
		case MENU:
			GameCont->hideAll();
			showMenus();
			break;
		case GAME:
			GameCont->showAll();
			hideMenus();
			break;
		}
	}
	void hideMenus() {
		States->setMenu(false);
		for (int i = 0; i < Menus.size(); i++) {
			for (int j = 0; j < Menus[i].size(); j++) {
				Menus[i][j]->hide();
			}
		}
	}
	void showMenus() {
		hideMenus();
		States->setMenu(true);
		if (Submenu < 0 || Menus.size() < Submenu) {
			qDebug() << "Submenu-index out of range!!!";
		} else {
			for (int j = 0; j < Menus[Submenu].size(); j++) {
				Menus[Submenu][j]->show();
			}
		}
	}
	void addMenusToWidget() {
		for (int i = 0; i < Menus.size(); i++) {
			for (int j = 0; j < Menus[i].size(); j++) {
				layout()->addWidget(Menus[i][j]);
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
		Menus.push_back(std::vector<QWidget*>());
	}

	void addMenuButton(QWidget *in) {
		Menus[Menus.size()-1].push_back(in);
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
			// This hack is needed because of glitchy supprt in Windows. It causes the OpenGL-window to disappear if it covers the entire viewable surface.
			res[i]->resize(CurrentSize.width(), CurrentSize.height()-1);
#else
			res[i]->resize(CurrentSize);
#endif
		}
	}
	void initSim() {
		Sim::Configuration &config = SimFull.getConfiguration();

		config.phaseLength = 10;
		config.stepTime = 0.01;
		SimFull.startup();

		//Sim.getData().getProgramDb().registerAllDefault();

		Sim::Player testSide;
		SimFull.getState().getPlayerData().addPlayer(testSide);

		//Sim.getData().getBotDb().addBot(myBot, cpts);
		// Create a test bot
		Sim::Bot::Config botCfg;
		botCfg.mSide = 0;
		//botCfg.mType = 0;
		botCfg.mBody.mPos = Sim::Vector(0,0);
		int botId = SimFull.getInput().getBotInput().buildInputImpl<Sim::Bot>(botCfg, 0)->getId();
		Sim::Bot::Config botCfg2;
		botCfg2.mSide = 0;
		//botCfg2.mType = 0;
		botCfg2.mBody.mPos = Sim::Vector(0,1);
		SimFull.getInput().getBotInput().buildInputImpl<Sim::Bot>(botCfg2, 0);
		Sim::Bot::Config botCfg3;
		botCfg3.mSide = 0;
		//botCfg3.mType = 0;
		botCfg3.mBody.mPos = Sim::Vector(0,2);
		SimFull.getInput().getBotInput().buildInputImpl<Sim::Bot>(botCfg3, 0);


		// Send some input to this bot
		{
			Sim::Input &inMgr = SimFull.getInput();
			Sim::ProgramFactory &progFact = SimFull.getState().getProgramFactory();

			using namespace Sim::Prog;
			Sim::IdType moveId = inMgr.getProgramInput().buildInputImplStr<MoveTowards>(
						MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(10,0)), "MoveTowards")->
					getId();

			//Kill *kill = inMgr.getProgramInput().buildInputImpl<Kill>(
			//			Kill::Config(moveId));
			inMgr.getCpuInput().registerInput(botId, moveId, 0);
			//inMgr.getCpuInput().registerInput(botId, kill->getId(), 20);
		}

		Sim::TileDatabase &db = SimFull.getData().getTileDb();
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

		//SimFull.getState().getWorld().getTile(3,0).setType(1);

		SimFull.prepareSim();

		States->setSim(&SimFull);
	}

protected:
	std::vector<QWidget*> res;
	// overridden
	void resizeEvent(QResizeEvent *event) {
		CurrentSize = event->size();
		fixRes();
	}
	void keyPressEvent(QKeyEvent *event) {
		int k = event->key();
		if (k == Qt::Key_F11) {
			FullScreen = !FullScreen;
			setFullScreen(FullScreen);
		} else if (k == Qt::Key_Escape) {
			toggleMenu();
		} else {
			if (States != NULL) {
				modKey(k, true);
			}
		}
	}

	void keyReleaseEvent(QKeyEvent *event) {
		if (States != NULL) {
			modKey(event->key(), false);
		}
	}
	void modKey(int k, bool state) {
		if ( k == Qt::Key_Shift ) {
			States->setShift(state);
		} else if ( k == Qt::Key_Control ) {
			States->setCtrl(state);
			if (state) {

			}
		}
	}

};

#endif
