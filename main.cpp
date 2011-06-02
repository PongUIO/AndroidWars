#include <QtGui>
#include <QWidget>
#include "main.h"
#include "test.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	GameUi *window;
	window = new GameUi(&app);
	window->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
	window->showFullScreen();
	//window->show();

	QPushButton *button = new QPushButton(
			QApplication::translate("childwidget", "Press me"), window);
	button->move(100, 100);

	button->show();
	//clicked();
	window->connect(button, SIGNAL(clicked()), window, SLOT(c()));
	return app.exec();
}

