#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include<QtGui>
#include<stdlib.h>
#include<typeinfo>

#include "Simulation.h"
#include "../openglqt/render.h"
#include "../util/client.h"
#include "customlabel.h"
#include "gamebutton.h"

#define RATE 20

void something() {
	qDebug() << "works.";
}

class GameController {
public:
	ClientStates *states;
	QWidget *parent;
	GameDrawer *drawer;
	QTimer *glTimer, *camTimer, *timer;
	QHBoxLayout *lower, *upper;
	QVBoxLayout *iconHolder;
	QSpacerItem *space;
	QPixmap profiles[1];
	GameButton *label;
	CustomLabel *label2, *robot;
	Sim::Simulation sim;
	Camera *cam;
	QPixmap m;
#ifdef WIN32
	QPalette *p;
#endif
	GameController(QWidget *parent = 0) {
		this->parent = parent;
		Sim::Configuration config;

		config.phaseLength = 10;
		config.stepTime = 0.01;

		sim.startup(config);

		Sim::Player testSide;
		sim.getState().getPlayerData().addPlayer(testSide);

		sim.getData().getWeaponDb().createType();

		Sim::BotD myBot;
		myBot.baseSpeed = 100.0;
		myBot.baseWeight = 75.0;

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

		// Create test weapon and bullet type
		Sim::BulletD *bulletType = sim.getData().getBulletDb().createType();

		Sim::StateSys *sys = bulletType->getStateSys();
		sys->registerState(new Sim::StdState::Delay(0.2));
		sys->registerEntryPoint(0);

		Sim::WeaponD *weaponType = sim.getData().getWeaponDb().createType();
		Sim::StateSys *wsys = weaponType->getStateSys();
		wsys->registerState(new Sim::WeaponState::Shoot( bulletType->getId() ) );
		wsys->registerEntryPoint(0);

		// Send some input to this bot
		Sim::BotFactory &botFact = sim.getState().getBotFactory();
		Sim::BotInput bi;
		bi = Sim::BotInput::inMove(botId, 20, Sim::Vector(1,0) );
		botFact.getInput().addInput( bi );

		//bi = Sim::BotInput::inShoot(botId, 0, Sim::Vector(0,1));
		//botFact.getInput().addInput( bi );

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


		//main = new QWidget(parent);
		//main->showFullScreen();
		//main->resize(parent->geometry().width(), parent->geometry().height());
		cam = new Camera(0, 0, parent->width(), parent->height());
		states = new ClientStates();
		drawer = new GameDrawer(cam, &sim, states, parent);

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

#ifdef _WIN32
		drawer->resize(parent->width(), parent->height()-1);
		p = new QPalette( parent->palette() );
		p->setColor( QPalette::Window, Qt::black );
		drawer->setPalette( *p );
#else
		drawer->resize(parent->width(), parent->height());
#endif
		label = new GameButton(&sim, 0, parent);
		label2 = new CustomLabel(&something, parent);
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
		std::vector<Sim::Bot*> bots = sim.getState().getBotFactory().getBotVector();
		CustomLabel *clabel;
		for (i = 0; i < bots.size(); i++) {
			clabel = new CustomLabel(&something, parent);
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
