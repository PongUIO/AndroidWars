#ifndef MENUBUTTON_H
#define MENUBUTTON_H
#include<QtGui>
class MenuButton : public QLabel {
	Q_OBJECT
public:
	int type, value;
	QWidget *parent;
	MenuButton(int type, int value, QWidget *parent = 0)
		: QLabel(parent) {
		this->parent = parent;
		this->value = value;
		this->type = type;
	}
signals:
	void onClick(int type, int value);
protected:
	// overridden
        void mousePressEvent(QMouseEvent * event) {
		emit onClick(type, value);
	}
};
#endif
