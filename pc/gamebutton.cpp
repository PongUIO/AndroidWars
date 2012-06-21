#include "gamebutton.h"
GameButton::GameButton(Sim::Simulation *sim, ClientStates *states, int func, GameSlider *gs, QWidget *parent)
	: QLabel(parent) {
	this->gs = gs;
	this->func = func;
	this->sim = sim;
	this->cs = states;
}
void GameButton::mousePressEvent(QMouseEvent * event) {
	if (func == 0) {
		if (!cs->getRunning()) {
			gs->setValue(0);
			sim->startPhase();
			cs->setRunning(true);
		}
	}
}
