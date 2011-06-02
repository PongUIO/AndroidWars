#include <QtGui>
#include <QWidget>
#include "main.h"
#include "ui.h"
#include "../openglqt/render.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	const int width = QApplication::desktop()->width();
	const int height = QApplication::desktop()->height();
	MyGLDrawer *drawer = new MyGLDrawer(0);
	drawer->showFullScreen();
	drawer->resize(width, height);
	drawer->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
	QPushButton *button = new QPushButton(
			QApplication::translate("childwidget", "Press me"), drawer);
	button->move(100, 100);

	button->show();
	//clicked();
	//window->connect(button, SIGNAL(clicked()), window, SLOT(c()));
	return app.exec();
}

