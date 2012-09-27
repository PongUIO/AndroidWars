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
			mSubMenu = RESUME;
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
		if (!mStates->getRunning()) {
			return;
		} else if ( mExtSim.getSim().hasPhaseStep() ) {
			mExtSim.getSim().step();
		} else {
			mExtSim.getSim().endPhase();
			mExtSim.getInput().commitReplay();
			mExtSim.getInput().postProcessInput();
			mExtSim.getInput().discardInput();
			mStates->setRunning(false);
		}
	}
public:
	QPalette *mPal;
	bool mFullScreen, mGameRunning;
	State mDispState;
	int mSubMenu;
	ClientStates *mStates;
	GameController *mGameController;
	exts::ExtSim mExtSim;
	QSize mCurrentSize;
	std::vector< std::vector<QWidget*> > mMenus;
	QTimer *mGameTimer;
	double mLastUpdate;
	QSettings *mSettings;
	MainWidget (QWidget *parent = 0) : QWidget(parent) {
		mSettings = new QSettings("PongUIO", "AndroidWars");
		mDispState = MENU;
		mSubMenu = MAIN;
		mStates = new ClientStates();
		loadFiles("../testmod/data/");
		initSim();
		mFullScreen = false;
		mPal = new QPalette( this->palette() );
		mPal->setColor( QPalette::Background, Qt::black );
		setPalette( *mPal );
		mGameRunning = false;
		mGameController = new GameController(mStates, this);
		mGameController->hideAll();
		registerForResize(mGameController->mDrawer);
		resize(700,700);
		initMenus();
		mGameTimer = new QTimer(this);
		connect(mGameTimer, SIGNAL(timeout()), this, SLOT(gameStep()));
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
			mExtSim.loadDataScript(std::string(f.readAll()));
			f.close();
		}
		mExtSim.postProcessData();

	}
	void resetTimer() {
		mGameTimer->stop();
		mGameTimer->start(mExtSim.getSim().getConfiguration().stepTime*1000);
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
		mStates = nS;
		mStates->releaseMods();

	}
	void changeToSubmenu(int value) {
		mSubMenu = value;
		showMenus();
	}
	void toggleMenu() {
		switch (mDispState) {
		case GAME:
			setState(MENU);
			break;
		case MENU:
			setState(GAME);
			break;
		}
	}
	void setState(State s) {
		mDispState = s;
		switch (s) {
		case MENU:
			mGameController->hideAll();
			showMenus();
			break;
		case GAME:
			mGameController->showAll();
			hideMenus();
			break;
		}
	}
	void hideMenus() {
		mStates->setMenu(false);
		for (int i = 0; i < mMenus.size(); i++) {
			for (int j = 0; j < mMenus[i].size(); j++) {
				mMenus[i][j]->hide();
			}
		}
	}
	void showMenus() {
		hideMenus();
		mStates->setMenu(true);
		if (mSubMenu < 0 || mMenus.size() < mSubMenu) {
			qDebug() << "Submenu-index out of range!!!";
		} else {
			for (int j = 0; j < mMenus[mSubMenu].size(); j++) {
				mMenus[mSubMenu][j]->show();
			}
		}
	}
	void addMenusToWidget() {
		for (int i = 0; i < mMenus.size(); i++) {
			for (int j = 0; j < mMenus[i].size(); j++) {
				layout()->addWidget(mMenus[i][j]);
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
		mMenus.push_back(std::vector<QWidget*>());
	}

	void addMenuButton(QWidget *in) {
		mMenus[mMenus.size()-1].push_back(in);
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
			res[i]->resize(mCurrentSize);
#endif
		}
	}
	void initSim() {
		Sim::Configuration &config = mExtSim.getSim().getConfiguration();

		config.phaseLength = 10;
		config.stepTime = 0.01;
		mExtSim.startup();

		//ExSim.getSim().getData().getProgramDb().registerAllDefault();
		mExtSim.getAgent().setupAgents(3);
		Sim::Player testSide;
		mExtSim.getSim().getState().getPlayerData().addPlayer(testSide);

		//ExSim.getSim().getData().getBotDb().addBot(myBot, cpts);
		// Create a test bot
		Sim::Bot::Config botCfg;
		botCfg.mSide = 0;
		//botCfg.mType = 0;
		botCfg.mBody.mPos = Sim::Vector(0,0);
		mExtSim.getSim().getState().getBotFactory().createImpl<Sim::Bot>(botCfg, 0, 0);
		Sim::Bot::Config botCfg2;
		botCfg2.mSide = 0;
		//botCfg2.mType = 0;
		botCfg2.mBody.mPos = Sim::Vector(0,1);
		mExtSim.getSim().getState().getBotFactory().createImpl<Sim::Bot>(botCfg2, 0, 1);
		Sim::Bot::Config botCfg3;
		botCfg3.mSide = 0;
		//botCfg3.mType = 0;
		botCfg3.mBody.mPos = Sim::Vector(0,2);
		mExtSim.getSim().getState().getBotFactory().createImpl<Sim::Bot>(botCfg3, 0, 2);



		Sim::TileDatabase &db = mExtSim.getSim().getData().getTileDb();
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

		//ExSim.getSim().getState().getWorld().getTile(3,0).setType(1);

		mExtSim.prepareSim();

		mStates->setExtSim(&mExtSim);
	}

protected:
	std::vector<QWidget*> res;
	// overridden
	void resizeEvent(QResizeEvent *event) {
		mCurrentSize = event->size();
		fixRes();
	}
	void keyPressEvent(QKeyEvent *event) {
		int k = event->key();
		if (k == Qt::Key_F11) {
			mFullScreen = !mFullScreen;
			setFullScreen(mFullScreen);
		} else if (k == Qt::Key_Escape) {
			toggleMenu();
		} else {
			if (mStates != NULL) {
				modKey(k, true);
			}
		}
	}

	void keyReleaseEvent(QKeyEvent *event) {
		if (mStates != NULL) {
			modKey(event->key(), false);
		}
	}

	void modKey(int k, bool state) {
		switch (k) {
		case Qt::Key_Shift:
			mStates->setShift(state);
			break;
		case Qt::Key_Control:
			mStates->setCtrl(state);
			break;
		case Qt::Key_W:
		case Qt::Key_A:
		case Qt::Key_S:
		case Qt::Key_D:
			mGameController->mDrawer->modKeyState(k, state);
			break;
		default:
			break;
		}
	}

};

#endif
