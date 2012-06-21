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
	GameController(ClientStates *states, QWidget *parent = 0);
	void showAll();
	void hideAll();
	void updateGUI();
	void emptyGUI();
	void changeBot(int id);
};
#endif // GAMECONTROLLER_H
