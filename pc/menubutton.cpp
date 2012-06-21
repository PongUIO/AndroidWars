#include "menudefines.h"
#include "menubutton.h"
MenuButton::MenuButton(State type, int value, QWidget *parent)
	: QPushButton(parent) {
	this->parent = parent;
	this->value = value;
	this->type = type;
	setFont(QFont("Arial", 30));
}
void MenuButton::mousePressEvent(QMouseEvent * event) {
	emit onClick(type, value);
}
