#ifndef MENUBUTTON_H
#define MENUBUTTON_H
#include<QtGui>
#include "menudefines.h"
class MenuButton : public QPushButton {
	Q_OBJECT
public:
	State mType;
	int mValue;
	QWidget *mParent;
	MenuButton(State mType, int mValue, QWidget *mParent = 0);
signals:
	void onClick(State mType, int mValue);
private:
	void mousePressEvent(QMouseEvent *e);
};

#endif // MENUBUTTON_H
