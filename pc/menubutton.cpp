#include "menudefines.h"
#include "menubutton.h"
MenuButton::MenuButton(State type, int value, QWidget *parent)
	: QPushButton(parent) {
	this->mParent = parent;
	this->mValue = value;
	this->mType = type;
	setFont(QFont("Arial", 30));
}
void MenuButton::mousePressEvent(QMouseEvent * event) {
	emit onClick(mType, mValue);
}
