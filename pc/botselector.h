#ifndef BOTSELECTOR_H
#define BOTSELECTOR_H
#include<QtGui>
#include "../util/client.h"
class BotSelector : public QLabel {
	Q_OBJECT
public:
	uint botId;
	ClientStates *states;
	BotSelector(uint botId, ClientStates *states, QWidget *parent = 0);
private:
	// overridden
	void mousePressEvent(QMouseEvent *ev);

};

#endif // BOTSELECTOR_H
