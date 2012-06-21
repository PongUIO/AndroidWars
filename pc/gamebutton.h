#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H
#include<QtGui>
#include<QThread>
#include "Simulation.h"
#include "gameslider.h"
#include "../util/client.h"

class GameButton : public QLabel {
	Q_OBJECT        // must include this if you use Qt signals/slots
public:
	int func;
	Sim::Simulation *sim;
	GameSlider *gs;
	ClientStates *cs;
	GameButton(Sim::Simulation *sim, ClientStates *states, int func, GameSlider *gs, QWidget *parent = 0);
private:
	// overridden
	void mousePressEvent(QMouseEvent *ev);
};
#endif // GAMEBUTTON_H
