#include <QtGui>
#include <QWidget>
#include <QAbstractButton>
#include <QIcon>
#include <QDebug>

#include "main.h"
#include "../openglqt/render.h"
#include "../util/camera.h"
#include "menubutton.h"
#include "Simulation.h"
#include "gamecontroller.h"

#ifndef RATE
#define RATE 60
#endif

int main(int argc, char *argv[]) {
        //QApplication::setGraphicsSystem(QLatin1String("opengl"));
        QApplication app(argc, argv);
        MainWidget *mainw = new MainWidget();
	mainw->show();
	mainw->setWindowTitle("AndroidWars - development");
	return app.exec();
}
