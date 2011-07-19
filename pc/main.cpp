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
#include "gamecontroller.h"

#ifndef RATE
#define RATE 60
#endif

GameController *test;

void gameStart() {
        test->showAll();
}

int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        QWidget *menu = new QWidget();
        CustomLabel *gameButton = new CustomLabel(&gameStart, menu);
        gameButton->setPixmap(QPixmap(":/graphics/menu/startgame.png"));
        gameButton->move(menu->geometry().width()/2, menu->geometry().width()/2);
        gameButton->show();
        menu->showFullScreen();
        test = new GameController(menu);
        test->hideAll();
        app.exec();
        delete test;
        delete gameButton;
        delete menu;
        return 0;//main.exec();
}
