#ifndef MAIN_H
#define MAIN_H

#include<QtGui>
#include<QWidget>
#include<QDebug>
#include<vector>
#include"../util/client.h"

class MainWidget : public QWidget {
public:
	QPalette *p;
	bool fullScreen;
	ClientStates *states;
	MainWidget (QWidget *parent = 0) : QWidget(parent) {
		grabKeyboard();
		states = new ClientStates();
		fullScreen = false;
		p = new QPalette( this->palette() );
		p->setColor( QPalette::Background, Qt::black );
		setPalette( *p );
	}
	void registerForResize(QWidget *target) {
		res.push_back(target);
	}
	void setFullScreen(bool state) {
		if (state) {
			showFullScreen();
		} else {
			showNormal();
		}
	}
	void setStates(ClientStates *nS) {
		states = nS;
		states->releaseMods();
	}
protected:
	std::vector<QWidget*> res;
	// overridden
	void resizeEvent(QResizeEvent *event) {
		resize(event->size());
		qDebug() << event->size();
		for (int i = 0; i < res.size(); i++) {
			#ifdef _WIN32
			res[i]->resize(event->size().width(), event->size().height()-1);
			#else
			res[i]->resize(event->size());
			#endif
		}
	}
	void keyPressEvent(QKeyEvent *event) {
		int k = event->key();
		if (k == Qt::Key_F11) {
			fullScreen = !fullScreen;
			setFullScreen(fullScreen);
		} else {
			if (states != NULL) {
				modKey(k, true);
			}
		}
	}

	void keyReleaseEvent(QKeyEvent *event) {
		if (states != NULL) {
			modKey(event->key(), false);
		}
	}
	void modKey(int k, bool state) {
		if ( k == Qt::Key_Shift ) {
			states->setShift(state);
		} else if ( k == Qt::Key_Control ) {
			states->setCtrl(state);
			if (state) {

			}
		}
	}
};

#endif
