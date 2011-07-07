#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H
#include<QtGui>
#include "Simulation.h"
#include "../openglqt/render.h"
#include "customlabel.h"

#define RATE 60

void something() {
        qDebug() << "works.";
}

class GameWidget : public QWidget {
public:
        QWidget *main;
        QTimer *glTimer, *camTimer, *timer;
        QHBoxLayout *lower;
        QSpacerItem *space;
        CustomLabel *label, *label2;
        Sim::Simulation sim;
        Camera *cam;
#ifdef WIN32
        QPalette *p;
#endif
        GameWidget(QWidget *parent = 0) : QWidget(parent) {

                Sim::Configuration config;

                config.phaseLength = 25;
                config.stepTime = 0.01;

                sim.startup(config);

                Sim::Player testSide;
                sim.getState().getPlayerData().addPlayer(testSide);
                // Create a test bot
                Sim::Bot::Config botCfg;
                botCfg.side = 0;
                botCfg.pos = Sim::Vector(0,0);
                uint32_t botId = sim.getState().getBotFactory().createBot( botCfg );

                // Send some input to this bot
                Sim::BotFactory &botFact = sim.getState().getBotFactory();


                Sim::TileDatabase &db = sim.getData().getTileDb();
                Sim::TileD myTile;
                myTile.colMask = 0;
                myTile.blastResist = 0.0;
                myTile.bounce = 0.0;
                myTile.friction = 0.0;
                db.addTile(myTile);
                db.addTile(myTile);



                main = new QWidget();
                main->showFullScreen();
                main->resize(QApplication::desktop()->screenGeometry().width(), QApplication::desktop()->screenGeometry().height());
                cam = new Camera(0, 0, main->width(), main->height());
                MyGLDrawer *drawer = new MyGLDrawer(cam, &sim, main);

                glTimer = new QTimer(main);
                drawer->connect(glTimer, SIGNAL(timeout()), drawer, SLOT(redraw()));
                glTimer->start(0);

                camTimer = new QTimer(main);
                drawer->connect(camTimer, SIGNAL(timeout()), drawer, SLOT(moveMouseCheck()));
                camTimer->start(1./RATE);

                lower = new QHBoxLayout(main);
                main->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
                drawer->setMouseTracking(true);

#ifdef _WIN32
                drawer->resize(main->width(), main->height()-1);
                p = new QPalette( main->palette() );
                p.setColor( QPalette::Window, Qt::black );
                main->setPalette( *p );
#else
                drawer->resize(main->width(), main->height());
#endif
                drawer->show();
                qDebug() << drawer->height();

                label = new CustomLabel(&something, main);
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
        }
};

#endif // GAMEWIDGET_H
