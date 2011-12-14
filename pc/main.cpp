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
	MainWidget *menu = new MainWidget();
	//menu->showFullScreen();
	//menu->resize(QApplication::desktop()->screenGeometry().width(),QApplication::desktop()->screenGeometry().height());
	menu->resize(700,700);
	menu->show();
	QPixmap m;
	m.convertFromImage(QImage(":/graphics/mouse/default.png").scaled(64,64));
	menu->setCursor(m);

	CustomLabel *gameButton = new CustomLabel(&gameStart, menu);
	gameButton->setPixmap(QPixmap(":/graphics/menu/startgame.png"));
	gameButton->move(menu->geometry().width()/2-gameButton->pixmap()->width()/2, menu->geometry().height()/2-gameButton->pixmap()->height()/2);
	gameButton->show();
	gc = new GameController(menu);
	gc->hideAll();
	menu->registerForResize(gc->drawer);
	app.exec();
	delete gc;
        delete gameButton;
	//delete menu;
	return 0;//main.exec();
}
