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
        main->showFullScreen();
        main->resize(QApplication::desktop()->screenGeometry().width(), QApplication::desktop()->screenGeometry().height());
        MyGLDrawer *drawer = new MyGLDrawer(main);
        QHBoxLayout lower(main);
        main->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
        drawer->setMouseTracking(true);

#ifdef _WIN32
        drawer->resize(main->width(), main->height()-1);
        QPalette p( main->palette() );
        p.setColor( QPalette::Window, Qt::black );
        main->setPalette( p );
#else
        drawer->resize(main->width(), main->height());
#endif
        drawer->show();
        qDebug() << drawer->height();

        CustomLabel *label = new CustomLabel(&something, main);
        CustomLabel *label2 = new CustomLabel(&something, main);

        label->setPixmap(QPixmap(":/graphics/temp/temp.png"));
        label->resize(60,60);
        label2->setPixmap(QPixmap(":/graphics/temp/temp2.png"));
        label2->resize(60,60);
        QSpacerItem *space = new QSpacerItem(main->width(), 0, QSizePolicy::Expanding);
	label->show();
	label2->show();
        lower.insertWidget(0, label);
	lower.insertWidget(1, label2);
	lower.insertSpacerItem(2, space);
	lower.setAlignment(Qt::AlignBottom);


        return app.exec();

}


