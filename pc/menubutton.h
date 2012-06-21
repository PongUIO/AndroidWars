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
	MenuButton(State type, int value, QWidget *parent = 0);
signals:
	void onClick(State type, int value);
private:
	void mousePressEvent(QMouseEvent *e);
};

#endif // MENUBUTTON_H
