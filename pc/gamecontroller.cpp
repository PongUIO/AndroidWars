#include "gamecontroller.h"

GameController::GameController(ClientStates *states, QWidget *parent) : states(states), parent(parent) {

	drawer = new GameDrawer(states, parent);
	drawer->setMouseTracking(true);
	iconHolder  = new QVBoxLayout(parent);
	lower = new QHBoxLayout();
	upper = new QHBoxLayout();
	iconHolder->insertLayout(0, lower);
	iconHolder->insertSpacing(1, -1);
	iconHolder->insertLayout(0, upper);

	slider = new GameSlider(states->getSim(), states, parent);

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
void GameController::showAll() {
	label->show();
	label2->show();
	drawer->show();
	drawer->startTimers();
	slider->show();
	updateGUI();
}
void GameController::hideAll() {
	label->hide();
	label2->hide();
	drawer->hide();
	drawer->stopTimers();
	slider->hide();
	emptyGUI();
}

void GameController::updateGUI() {
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

void GameController::emptyGUI() {
	QLayoutItem *temp;
	while ((temp = upper->takeAt(0))) {
		delete temp->widget();
	}
}
void GameController::changeBot(int bot) {
	updateGUI();
}
