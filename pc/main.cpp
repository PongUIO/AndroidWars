#include <QtGui>
#include <QWidget>
#include <QAbstractButton>
#include <QIcon>
<<<<<<< HEAD
#include <QDebug>
=======
>>>>>>> origin/master
#include <iostream>
#include "main.h"
#include "ui.h"
#include "customlabel.h"
#include "../openglqt/render.h"
void something() {
	std::cout << "works." << std::endl;
}

int main(int argc, char *argv[]) {
<<<<<<< HEAD
        QApplication app(argc, argv);
=======
	QApplication app(argc, argv);
>>>>>>> origin/master
	MyGLDrawer *drawer = new MyGLDrawer(0);
	drawer->showFullScreen();
	drawer->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
	drawer->setMouseTracking(true);
<<<<<<< HEAD
        QHBoxLayout lower(drawer);
        CustomLabel *label = new CustomLabel(&something, drawer);
        CustomLabel *label2 = new CustomLabel(&something, drawer);
        QPixmap pixmap;
        if (!pixmap.load( ":/images/temp.png" )) {
            qWarning("Failed to load images/temp.png");
        }

        label->setPixmap(QPixmap(":/images/temp.png"));
        label->resize(60,60);
        label2->setPixmap(QPixmap(":/images/temp2.png"));
	label2->resize(60,60);
=======
	QHBoxLayout lower(drawer);
	CustomLabel *label = new CustomLabel(&something, drawer);
	CustomLabel *label2 = new CustomLabel(&something, drawer);
	label->setPixmap(QPixmap("temp.png"));
	label->resize(60,60);
	label2->setPixmap(QPixmap("temp2.png"));
	label2->resize(60,60);
        QPushButton *button = new QPushButton(drawer);
        button->move(100, 100);
        button->show ();
>>>>>>> origin/master
	QSpacerItem *space = new QSpacerItem(drawer->width(), 0, QSizePolicy::Expanding);
	//space->setOrientation(Qt::Horizontal);
	label->show();
	label2->show();
	//label->move(200,200);
	//label2->move(300,200);
	lower.insertWidget(0, label);
	lower.insertWidget(1, label2);
<<<<<<< HEAD
	lower.insertSpacerItem(2, space);
	std::cout << space->expandingDirections() << std::endl;
	lower.setAlignment(Qt::AlignBottom);
	//lower.move(drawer->height()-50, 0);
        //lower.show();
=======
        //lower.insertSpacerItem(2, space);
	std::cout << space->expandingDirections() << std::endl;
	lower.setAlignment(Qt::AlignBottom);
	//lower.move(drawer->height()-50, 0);
	//lower.show();
>>>>>>> origin/master
	//clicked();
	//drawer->connect(label, SIGNAL(clicked()), label, SLOT(quit()));
	return app.exec();
}


