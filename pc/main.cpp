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
/*        QGraphicsView view;
        view.setViewport(new QGLWidget(
            QGLFormat(QGL::SampleBuffers)));
        view.setViewportUpdateMode(
            QGraphicsView::FullViewportUpdate);
        view.setScene(new QGraphicsScene);
        view.scene()->addWidget(mainw);
        view.show();*/
        MainWidget *mainw = new MainWidget();
        mainw->show();
	app.exec();
	//delete menu;
	return 0;//main.exec();
}
