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
	QWidget *mParent;
	GameDrawer *mDrawer;
	QTimer *mCamTimer, *mTimer;
	QHBoxLayout *mLower, *mUpper;
	QVBoxLayout *mIconHolder;
	QSpacerItem *mSpace;
	QPixmap mProfiles[1];
	ClientStates *mStates;

	//buttons
	GameButton *mLabel1, *mLabel2;
	GameSlider *mSlider;
	GameController(ClientStates *mStates, QWidget *mParent = 0);
	void showAll();
	void hideAll();
	void updateGUI();
	void emptyGUI();
	void changeBot(int id);
};
#endif // GAMECONTROLLER_H
