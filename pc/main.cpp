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
#include "gamewidget.h"

#ifndef RATE
#define RATE 60
#endif


void gameStart() {

}

int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        int *currentChoice = new int(0);
        QWidget *menu = new QWidget();
        CustomLabel *gameButton = new CustomLabel(&gameStart, menu);
        gameButton->setPixmap(QPixmap(":/graphics/menu/startgame.png"));
        gameButton->move(menu->geometry().width()/2, menu->geometry().width()/2);
        gameButton->show();
        menu->showFullScreen();
        GameWidget *test = new GameWidget(menu);
        app.exec();
        return 0;//main.exec();
}
