#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include<QtGui>
#include<stdlib.h>
#include<typeinfo>

#include "Simulation.h"
#include "../openglqt/render.h"
#include "../util/client.h"
#include "botselector.h"
#include "gamebutton.h"

#define RATE 60

class GameController {
public:
	QWidget *parent;
	GameDrawer *drawer;
	QTimer *glTimer, *camTimer, *timer;
	QHBoxLayout *lower, *upper;
	QVBoxLayout *iconHolder;
	QSpacerItem *space;
	QPixmap profiles[1];
	GameButton *label, *label2;
	QPixmap m;
	ClientStates *states;
	GameController(ClientStates *states, QWidget *parent = 0) {
		this->states = states;
		this->parent = parent;

		drawer = new GameDrawer(states, parent);

		glTimer = new QTimer(parent);
		drawer->connect(glTimer, SIGNAL(timeout()), drawer, SLOT(redraw()));
		glTimer->start(0);

		camTimer = new QTimer(parent);
                drawer->connect(camTimer, SIGNAL(timeout()), drawer, SLOT(tick()));
                camTimer->start(1000./RATE);

		iconHolder  = new QVBoxLayout(parent);
		lower = new QHBoxLayout();
		upper = new QHBoxLayout();
		iconHolder->insertLayout(0, lower);
		iconHolder->insertSpacing(1, -1);
		iconHolder->insertLayout(0, upper);
		parent->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
		drawer->setMouseTracking(true);

		label = new GameButton(states->getSim(), 0, parent);
		label2 = new GameButton(states->getSim(), 0, parent);
		label->setPixmap(QPixmap(":/graphics/temp/temp.png"));
		label->resize(60,60);
		label2->setPixmap(QPixmap(":/graphics/temp/temp2.png"));
		label2->resize(60,60);
		profiles[0] = QPixmap(":/graphics/profiles/test.png");
		space = new QSpacerItem(parent->width(), 0, QSizePolicy::Expanding);
		label->show();
		label2->show();
		lower->insertWidget(0, label);
		lower->insertWidget(1, label2);
		lower->insertSpacerItem(2, space);
		lower->setAlignment(Qt::AlignBottom);
		upper->setAlignment(Qt::AlignTop);
		updateGUI();
    }
	void showAll() {
		label->show();
		label2->show();
		drawer->show();
		updateGUI();
	}
	void hideAll() {
		label->hide();
		label2->hide();
		drawer->hide();
		emptyGUI();
	}

	void updateGUI() {
		emptyGUI();
		int i;
		std::list<Sim::Bot*> bots = states->getSim()->getState().getBotFactory().getBotList();
		std::list<Sim::Bot*>::iterator it;
		BotSelector *clabel;
		for (it = bots.begin(); it != bots.end(); it++) {
			clabel = new BotSelector((*it)->getId(), states, parent);
			clabel->setPixmap(profiles[0]);
			clabel->resize(60,60);
			upper->insertWidget(i, clabel);
			clabel->show();
		}
		upper->insertSpacerItem(i, space);
	}

	void emptyGUI() {
		QLayoutItem *temp;
		while ((temp = upper->takeAt(0))) {
			delete temp->widget();
		}
	}
	void changeBot(int bot) {
		updateGUI();
	}
};

#endif // GAMECONTROLLER_H
