#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include<QtGui>
#include<stdlib.h>
#include<typeinfo>

#include "Simulation.h"
#include "../openglqt/render.h"
#include "customlabel.h"
#include "gamebutton.h"

#define RATE 20

void something() {
	qDebug() << "works.";
}

class GameController {
public:
	QWidget *main;
	QTimer *glTimer, *camTimer, *timer;
        QHBoxLayout *lower, *upper;
        QVBoxLayout *iconHolder;
        QSpacerItem *space;
        QPixmap profiles[1];
	GameButton *label;
        CustomLabel *label2, *robot;
	Sim::Simulation sim;
	Camera *cam;
#ifdef WIN32
	QPalette *p;
#endif
	GameController(QWidget *parent = 0) {

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
		cpts.push_back(Sim::Vector(0,1));
		cpts.push_back(Sim::Vector(1,1));
		cpts.push_back(Sim::Vector(1,0));
		
		sim.getData().getBotDb().addBot(myBot, cpts);
		
		// Create a test bot
		Sim::Bot::Config botCfg;
		botCfg.side = 0;
		botCfg.type = 0;
		botCfg.pos = Sim::Vector(0,0);
		uint32_t botId = sim.getState().getBotFactory().createBot( botCfg );
		botCfg.pos = Sim::Vector(0,1);
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
		
		bi = Sim::BotInput::inShoot(botId, 0, Sim::Vector(0,1));
		botFact.getInput().addInput( bi );
		
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


		main = new QWidget(parent);
		main->showFullScreen();
                main->resize(parent->geometry().width(), parent->geometry().height());
		cam = new Camera(0, 0, main->width(), main->height());
		MyGLDrawer *drawer = new MyGLDrawer(cam, &sim, main);

		glTimer = new QTimer(main);
		drawer->connect(glTimer, SIGNAL(timeout()), drawer, SLOT(redraw()));
		glTimer->start(0);

		camTimer = new QTimer(main);
                drawer->connect(camTimer, SIGNAL(timeout()), drawer, SLOT(moveMouseCheck()));
                camTimer->start(1000./RATE);

                iconHolder  = new QVBoxLayout(main);
                lower = new QHBoxLayout();
                upper = new QHBoxLayout();
                iconHolder->insertLayout(0, lower);
                iconHolder->insertSpacing(1, -1);
                iconHolder->insertLayout(0, upper);
                main->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
		drawer->setMouseTracking(true);

#ifdef _WIN32
		drawer->resize(main->width(), main->height()-1);
		p = new QPalette( main->palette() );
		p->setColor( QPalette::Window, Qt::black );
		main->setPalette( *p );
#else
		drawer->resize(main->width(), main->height());
#endif
		drawer->show();

		label = new GameButton(&sim, 0, main);
		label2 = new CustomLabel(&something, main);
		label->setPixmap(QPixmap(":/graphics/temp/temp.png"));
		label->resize(60,60);
		label2->setPixmap(QPixmap(":/graphics/temp/temp2.png"));
		label2->resize(60,60);
		profiles[0] = QPixmap(":/graphics/profiles/test.png");
		space = new QSpacerItem(main->width(), 0, QSizePolicy::Expanding);
		label->show();
		label2->show();
		lower->insertWidget(0, label);
		lower->insertWidget(1, label2);
		lower->insertSpacerItem(2, space);
                lower->setAlignment(Qt::AlignBottom);
                updateGUI();
                botCfg.pos = Sim::Vector(0,2);
                sim.getState().getBotFactory().createBot( botCfg );
                updateGUI();
                upper->setAlignment(Qt::AlignTop);
	}
	void showAll() {
		main->show();
		label->show();
		label2->show();
	}
	void hideAll() {
		main->hide();
		label->hide();
		label2->hide();
	}

        void updateGUI() {
                QLayoutItem *temp;
                while ((temp = upper->takeAt(0))) {
                        delete temp->widget();
                }
                int i;
                std::vector<Sim::Bot*> bots = sim.getState().getBotFactory().getBotVector();
                CustomLabel *clabel;
                for (i = 0; i < bots.size(); i++) {
                        clabel = new CustomLabel(&something, main);
                        clabel->setPixmap(profiles[0]);
                        clabel->resize(60,60);
                        clabel->show();
                        upper->insertWidget(i, clabel);
                }
                upper->insertSpacerItem(i, space);
        }

        void changeBot(int bot) {
                updateGUI();
        }
};

#endif // GAMEWIDGET_H
