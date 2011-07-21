#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include<QtGui>
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
	GameButton *label;
        CustomLabel *label2, *robot;
	Sim::Simulation sim;
	Camera *cam;
#ifdef WIN32
	QPalette *p;
#endif
	GameController(QWidget *parent = 0) {

		Sim::Configuration config;

		config.phaseLength = 1;
		config.stepTime = 0.01;

		sim.startup(config);

		Sim::Player testSide;
		sim.getState().getPlayerData().addPlayer(testSide);
		
		Sim::BotD myBot;
		myBot.baseSpeed = 1.0;
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

		// Send some input to this bot
		Sim::BotFactory &botFact = sim.getState().getBotFactory();
		Sim::BotInput bi;
		bi = Sim::BotInput::inMove(botId, 20, Sim::Vector(1,0) );
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
		camTimer->start(1./RATE);

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
		space = new QSpacerItem(main->width(), 0, QSizePolicy::Expanding);
		label->show();
		label2->show();
		lower->insertWidget(0, label);
		lower->insertWidget(1, label2);
		lower->insertSpacerItem(2, space);
		lower->setAlignment(Qt::AlignBottom);
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
                int i = 0;
                robot = new CustomLabel(&something, main);
                robot->setPixmap(QPixmap(":/graphics/profiles/test.xcf"));
                robot->resize(60,60);
                upper->insertWidget(0, robot);
                upper->insertSpacerItem(2, space);
                robot->show();
        }

        void changeBot(int bot) {

        updateGUI();
        }
};

#endif // GAMEWIDGET_H
