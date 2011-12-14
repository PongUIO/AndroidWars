#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include<QtGui>
class CustomLabel : public QLabel {
	Q_OBJECT
public:
	void (*funcPointer)();
	CustomLabel(void (*in)(), QWidget *parent = 0)
		: QLabel(parent) {
		funcPointer = in;
	}
protected:
	// overridden
	void mousePressEvent(QMouseEvent * event) {
		funcPointer();
	}
};
#endif
