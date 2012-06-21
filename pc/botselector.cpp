#include "botselector.h"
BotSelector::BotSelector(uint botId, ClientStates *states, QWidget *parent)
	: QLabel(parent) {
	this->botId = botId;
	this->states = states;
}

// overridden
void BotSelector::mousePressEvent(QMouseEvent * event) {
	states->select(botId);
}
