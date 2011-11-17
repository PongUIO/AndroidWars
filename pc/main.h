#ifndef MAIN_H
#define MAIN_H

#include<QtGui>
#include<QWidget>
#include<QDebug>
#include<vector>

class MainWidget : public QWidget {
	public:
		void registerForResize(QWidget *target) {
			res.push_back(target);
		}
	protected:
		std::vector<QWidget*> res;
		// overridden
		void resizeEvent(QResizeEvent *event) {
			resize(event->size());
			for (int i = 0; i < res.size(); i++) {
				res[i]->resize(event->size());
			}
		}
};

#endif
