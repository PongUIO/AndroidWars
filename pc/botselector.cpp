#include "botselector.h"
BotSelector::BotSelector(uint botId, ClientStates *states, QWidget *parent)
	: QLabel(parent) {
	this->mBotId = botId;
	this->mStates = states;
}

// overridden
void BotSelector::mousePressEvent(QMouseEvent * event) {
	mStates->select(mBotId);
}
