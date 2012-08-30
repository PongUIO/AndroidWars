#include "gamebutton.h"
GameButton::GameButton(exts::ExtSim *sim, ClientStates *states, int func, GameSlider *gs, QWidget *parent)
	: QLabel(parent) {
	this->mGs = gs;
	this->mFunc = func;
	this->mExtSim = sim;
	this->mCs = states;
}
void GameButton::mousePressEvent(QMouseEvent * event) {
	if (mFunc == 0) {
		if (!mCs->getRunning()) {
			mExtSim->getInput().feedInput();
			mExtSim->getSim().startPhase();
			mGs->setValue(0);
			mCs->setRunning(true);
		}
	}
}
