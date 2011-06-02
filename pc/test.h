#include<QtGui>
#include "ui_test.h"
class GameUi : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT
	public:
		GameUi(QApplication *app, QWidget *parent=0);
};
