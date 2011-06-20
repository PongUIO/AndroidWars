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
void something() {
	std::cout << "works." << std::endl;
}

int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        QWidget *main = new QWidget();
        MyGLDrawer *drawer = new MyGLDrawer(main);
        main->showFullScreen();


        drawer->resize(main->width(), main->height()-1);
        QHBoxLayout lower(drawer);
        main->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
	drawer->setMouseTracking(true);

        //QWidget *widg = new QWidget(0);
        //QPushButton *button = new QPushButton(main);
        CustomLabel *label = new CustomLabel(&something, main);
        CustomLabel *label2 = new CustomLabel(&something, main);
        QPixmap pixmap;
        if (!pixmap.load( ":/images/temp.png" )) {
            qWarning("Failed to load images/temp.png");
        }



        label->setPixmap(QPixmap(":/images/temp.png"));
        label->resize(60,60);
        label2->setPixmap(QPixmap(":/images/temp2.png"));
	label2->resize(60,60);
        QSpacerItem *space = new QSpacerItem(main->width(), 0, QSizePolicy::Expanding);
	//space->setOrientation(Qt::Horizontal);
        //widg->show();
        //button->move(100,100);
        //button->show();
	label->show();
	label2->show();
	//label->move(200,200);
	//label2->move(300,200);
        lower.insertWidget(0, label);
	lower.insertWidget(1, label2);
	lower.insertSpacerItem(2, space);
	std::cout << space->expandingDirections() << std::endl;
	lower.setAlignment(Qt::AlignBottom);
                drawer->show();
	//lower.move(drawer->height()-50, 0);
        //lower.show();
	//clicked();
	//drawer->connect(label, SIGNAL(clicked()), label, SLOT(quit()));
        return app.exec();

}


