#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include<QtGui>
class MenuButton : public QLabel {
        Q_OBJECT        // must include this if you use Qt signals/slots
public:
        int *ptr;
        int changeTo;
        MenuButton(int *target, int ch, QWidget *parent = 0)
                : QLabel(parent) {
                        ptr = target;
                        changeTo = ch;
                }
protected:
        // overridden
        void mousePressEvent(QMouseEvent * event) {
                *ptr = changeTo;
        }
};
#endif // MENUBUTTON_H
