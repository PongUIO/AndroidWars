#ifndef MAIN_H
#define MAIN_H

#include<QtGui>
#include<QWidget>
#include<QDebug>
#include<vector>

class MainWidget : public QWidget {
	public:
		QPalette *p;
		MainWidget (QWidget *parent = 0) : QWidget(parent) {
			grabKeyboard();
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
};

#endif
