#ifndef BOTSELECTOR_H
#define BOTSELECTOR_H
#include<QtGui>
#include "../util/client.h"
class BotSelector : public QLabel {
	Q_OBJECT
public:
	uint mBotId;
	ClientStates *mStates;
	BotSelector(uint mBotId, ClientStates *mStates, QWidget *parent = 0);
private:
	// overridden
	void mousePressEvent(QMouseEvent *ev);

};

#endif // BOTSELECTOR_H
