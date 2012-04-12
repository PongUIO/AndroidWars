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
#include "gameslider.h"

class GameController {
public:
	QWidget *parent;
	GameDrawer *drawer;
	QTimer *camTimer, *timer;
	QHBoxLayout *lower, *upper;
	QVBoxLayout *iconHolder;
	QSpacerItem *space;
	QPixmap profiles[1];
	QPixmap m;
	ClientStates *states;

	//buttons
	GameButton *label, *label2;
	GameSlider *slider;
        GameController(ClientStates *states, QWidget *parent = 0) {
		this->states = states;
                this->parent = parent;

               drawer = new GameDrawer(states, parent);
                drawer->setMouseTracking(true);
                iconHolder  = new QVBoxLayout(parent);
		lower = new QHBoxLayout();
		upper = new QHBoxLayout();
		iconHolder->insertLayout(0, lower);
		iconHolder->insertSpacing(1, -1);
		iconHolder->insertLayout(0, upper);
		parent->setWindowTitle(QApplication::translate("childwidget", "Child widget"));

                slider = new GameSlider(states->getSim(), parent);

		label = new GameButton(states->getSim(), states, 0, slider, parent);
		label2 = new GameButton(states->getSim(), states, 0, slider, parent);


		label->setPixmap(QPixmap("../testmod/graphics/temp/temp.png"));
                label->resize(60,60);
		label2->setPixmap(QPixmap("../testmod/graphics/temp/temp2.png"));
		label2->resize(60,60);
		profiles[0] = QPixmap("../testmod/graphics/profiles/test.png");
		space = new QSpacerItem(parent->width(), 0, QSizePolicy::Expanding);
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
                drawer->startTimers();
                slider->show();
                updateGUI();
	}
	void hideAll() {
                label->hide();
                label2->hide();
                drawer->hide();
                drawer->stopTimers();
                slider->hide();
                emptyGUI();
        }

	void updateGUI() {
		emptyGUI();
		//upper->insertWidget(1, slider);
		upper->insertSpacerItem(0, space);
		int i = 1;
		int selected = states->getSelSize();
		std::list<Sim::Bot*> bots = states->getSim()->getState().getBotFactory().getBotList();
		std::list<Sim::Bot*>::iterator it;
		BotSelector *clabel;
		for (it = bots.begin(); it != bots.end(); it++) {
			clabel = new BotSelector((*it)->getId(), states, parent);
			clabel->setPixmap(profiles[0]);
			clabel->resize(60,60);
			upper->insertWidget(i, clabel);
			clabel->show();
			i++;
			if (selected == 1 && states->isSelected((*it)->getId())) {
			}
		}
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
