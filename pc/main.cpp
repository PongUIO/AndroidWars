#include <QtGui>
#include <QWidget>
#include <QAbstractButton>
#include <QIcon>
#include "main.h"
#include "ui.h"
#include "../openglqt/render.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MyGLDrawer *drawer = new MyGLDrawer(0);
	drawer->showFullScreen();
	drawer->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
	drawer->setMouseTracking(true);
	QIcon icon("temp.png");
	QAbstractButton *button = new QPushButton(drawer);
//			QApplication::translate("childwidget", "Press me"), drawer);
//	button->setStyleSheet("* { image: url(temp.png);}");
	button->setIcon(icon);
	button->move(100, 100);
//	button->setFlat(true);
	button->show();
	//clicked();
	//window->connect(button, SIGNAL(clicked()), window, SLOT(c()));
	return app.exec();
}

