#include <QtGui>
#include <QWidget>
#include <QAbstractButton>
#include <QIcon>
#include <QDebug>
#include <iostream>
#include "main.h"
#include "ui.h"
#include "customlabel.h"
#include "../openglqt/render.h"
#include "../util/camera.h"
#include "Simulation.h"

void something() {
	std::cout << "works." << std::endl;
}

int main(int argc, char *argv[]) {
        Sim::Simulation sim;
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
        int i = db.addTile(myTile);
        db.addTile(myTile);



        QApplication app(argc, argv);
        QWidget *main = new QWidget();
        main->showFullScreen();
        main->resize(QApplication::desktop()->screenGeometry().width(), QApplication::desktop()->screenGeometry().height());
        Camera cam(0, 0, main->width(), main->height());
        MyGLDrawer *drawer = new MyGLDrawer(&cam, &sim, main);

        QTimer *glTimer = new QTimer(main);
        drawer->connect(glTimer, SIGNAL(timeout()), drawer, SLOT(redraw()));
        glTimer->start(0);

        QTimer *timer = new QTimer(main);
        //main->connect(timer, SIGNAL(timeout()), &cam, SLOT(iter()));
        timer->start(100);

        QHBoxLayout lower(main);
        main->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
        drawer->setMouseTracking(true);

#ifdef _WIN32
        drawer->resize(main->width(), main->height()-1);
        QPalette p( main->palette() );
        p.setColor( QPalette::Window, Qt::black );
        main->setPalette( p );
#else
        drawer->resize(main->width(), main->height());
#endif
        drawer->show();
        qDebug() << drawer->height();

        CustomLabel *label = new CustomLabel(&something, main);
        CustomLabel *label2 = new CustomLabel(&something, main);

        label->setPixmap(QPixmap(":/graphics/temp/temp.png"));
        label->resize(60,60);
        label2->setPixmap(QPixmap(":/graphics/temp/temp2.png"));
        label2->resize(60,60);
        QSpacerItem *space = new QSpacerItem(main->width(), 0, QSizePolicy::Expanding);
	label->show();
	label2->show();
        lower.insertWidget(0, label);
	lower.insertWidget(1, label2);
	lower.insertSpacerItem(2, space);
	lower.setAlignment(Qt::AlignBottom);


        return app.exec();

}
