#include <QtGui>
#include <QWidget>
#include <QAbstractButton>
#include <QIcon>
#include <QDebug>

#include "main.h"
#include "ui.h"
#include "../openglqt/render.h"
#include "../util/camera.h"
#include "menubutton.h"
#include "Simulation.h"
#include "gamecontroller.h"

#ifndef RATE
#define RATE 60
#endif

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MainWidget *mainw = new MainWidget();
	mainw->show();
	app.exec();
	//delete menu;
	return 0;//main.exec();
}
