#ifndef MENUBUTTON_H
#define MENUBUTTON_H
#include<QtGui>
#include "menudefines.h"
class MenuButton : public QPushButton {
	Q_OBJECT
public:
        State type;
        int value;
	QWidget *parent;
        MenuButton(State type, int value, QWidget *parent = 0)
                : QPushButton(parent) {
		this->parent = parent;
		this->value = value;
                this->type = type;
                setFont(QFont("Arial", 30));
	}
signals:
        void onClick(State type, int value);
protected:
	// overridden
        void mousePressEvent(QMouseEvent * event) {
		emit onClick(type, value);
	}
};
#endif
