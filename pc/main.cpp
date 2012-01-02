#include <QtGui>
#include <QWidget>
#include <QAbstractButton>
#include <QIcon>
#include <QDebug>

#include "main.h"
#include "ui.h"
#include "customlabel.h"
#include "../openglqt/render.h"
#include "../util/camera.h"
#include "menubutton.h"
#include "Simulation.h"
#include "gamecontroller.h"

#ifndef RATE
#define RATE 60
#endif

GameController *gc;

void gameStart() {
	gc->showAll();
}

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MainWidget *mainw = new MainWidget();
	mainw->show();
	QPixmap m;
	m.convertFromImage(QImage(":/graphics/mouse/default.png").scaled(64,64));
	mainw->setCursor(m);

	CustomLabel *gameButton = new CustomLabel(&gameStart, mainw);
	gc = new GameController(mainw);
	gc->hideAll();
	mainw->registerForResize(gc->drawer);
	mainw->resize(700,700);
	gameButton->setPixmap(QPixmap(":/graphics/menu/startgame.png"));
	gameButton->move(mainw->geometry().width()/2-gameButton->pixmap()->width()/2, mainw->geometry().height()/2-gameButton->pixmap()->height()/2);
	gameButton->resize(gameButton->pixmap()->size());
	gameButton->show();
	app.exec();
	delete gc;
        delete gameButton;
	//delete menu;
	return 0;//main.exec();
}
