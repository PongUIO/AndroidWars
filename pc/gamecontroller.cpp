#include "gamecontroller.h"

GameController::GameController(ClientStates *states, QWidget *parent) : mStates(states), mParent(parent) {

	mDrawer = new GameDrawer(states, parent);
	mDrawer->setMouseTracking(true);
	mIconHolder  = new QVBoxLayout(parent);
	mLower = new QHBoxLayout();
	mUpper = new QHBoxLayout();
	mIconHolder->insertLayout(0, mLower);
	mIconHolder->insertSpacing(1, -1);
	mIconHolder->insertLayout(0, mUpper);

	mSlider = new GameSlider(states->getSim(), states, parent);

	mLabel1 = new GameButton(states->getSim(), states, 0, mSlider, parent);
	mLabel2 = new GameButton(states->getSim(), states, 0, mSlider, parent);


	mLabel1->setPixmap(QPixmap("../testmod/graphics/temp/temp.png"));
	mLabel1->resize(60,60);
	mLabel2->setPixmap(QPixmap("../testmod/graphics/temp/temp2.png"));
	mLabel2->resize(60,60);
	mProfiles[0] = QPixmap("../testmod/graphics/profiles/test.png");
	mSpace = new QSpacerItem(parent->width(), 0, QSizePolicy::Expanding);
	mLower->insertWidget(0, mLabel1);
	mLower->insertWidget(1, mLabel2);
	mLower->insertSpacerItem(2, mSpace);
	mLower->setAlignment(Qt::AlignBottom);
	mUpper->setAlignment(Qt::AlignTop);
	updateGUI();
}
void GameController::showAll() {
	mLabel1->show();
	mLabel2->show();
	mDrawer->show();
	mDrawer->startTimers();
	mSlider->show();
	updateGUI();
}
void GameController::hideAll() {
	mLabel1->hide();
	mLabel2->hide();
	mDrawer->hide();
	mDrawer->stopTimers();
	mSlider->hide();
	emptyGUI();
}

void GameController::updateGUI() {
	emptyGUI();
	//upper->insertWidget(1, slider);
	mUpper->insertSpacerItem(0, mSpace);
	int i = 1;
	int selected = mStates->getSelSize();
	std::list<Sim::Bot*> bots = mStates->getSim()->getSim().getState().getBotFactory().getBotList();
	std::list<Sim::Bot*>::iterator it;
	BotSelector *clabel;
	for (it = bots.begin(); it != bots.end(); it++) {
		clabel = new BotSelector((*it)->getId(), mStates, mParent);
		clabel->setPixmap(mProfiles[0]);
		clabel->resize(60,60);
		mUpper->insertWidget(i, clabel);
		clabel->show();
		i++;
		if (selected == 1 && mStates->isSelected((*it)->getId())) {
		}
	}
}

void GameController::emptyGUI() {
	QLayoutItem *temp;
	while ((temp = mUpper->takeAt(0))) {
		delete temp->widget();
	}
}
void GameController::changeBot(int bot) {
	updateGUI();
}
