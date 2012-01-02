#ifndef BOTSELECTOR_H
#define BOTSELECTOR_H
#include<QtGui>
#include "../util/client.h"
class BotSelector : public QLabel {
	Q_OBJECT
public:
	uint botId;
	ClientStates *states;
	BotSelector(uint botId, ClientStates *states, QWidget *parent = 0)
		: QLabel(parent) {
		this->botId = botId;
		this->states = states;
	}
protected:
	// overridden
	void mousePressEvent(QMouseEvent * event) {
		states->select(botId);
	}
};
#endif
