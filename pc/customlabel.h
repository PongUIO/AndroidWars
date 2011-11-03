#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include<QtGui>
class CustomLabel : public QLabel {
	Q_OBJECT        // must include this if you use Qt signals/slots
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
