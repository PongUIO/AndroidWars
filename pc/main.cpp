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
	QHBoxLayout lower(drawer);
	QAbstractButton *button = new QPushButton(drawer);
	QAbstractButton *button2 = new QPushButton(drawer);
	QSpacerItem *space = new QSpacerItem(drawer->width()+1000, 0);
//			QApplication::translate("childwidget", "Press me"), drawer);
//	button->setStyleSheet("* { image: url(temp.png);}");
	button->setIcon(icon);
	button2->setIcon(icon);
//	button->setFlat(true);
//	button->move(100, 100);
	button->show();
	button2->show();
	lower.insertWidget(0, button);
	lower.insertWidget(1, button2);
	lower.insertSpacerItem(2, space);
	lower.setAlignment(Qt::AlignBottom);
	//lower.move(drawer->height()-50, 0);
	//lower.show();
	//clicked();
	//window->connect(button, SIGNAL(clicked()), window, SLOT(c()));
	return app.exec();
}

