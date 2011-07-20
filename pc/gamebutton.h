#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H
#include<QtGui>
#include "Simulation.h"
class GameButton : public QLabel {
	Q_OBJECT        // must include this if you use Qt signals/slots
public:
	int func;
	Sim::Simulation *sim;
	GameButton(Sim::Simulation *sim, int func, QWidget *parent = 0)
		: QLabel(parent) {
		this->func = func;
		this->sim = sim;
	}
protected:
	// overridden
        void mousePressEvent(QMouseEvent * event) {
                if (func == 0) {
			sim->startPhase();
			while( sim->hasPhaseStep() ) {
				sim->step();
			}
			sim->endPhase();
		}
	}
};
#endif
