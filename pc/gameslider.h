#ifndef GAMESLIDER_H
#define GAMESLIDER_H
#include<QSlider>
#include<QStyle>
#include<QLineEdit>
#include "Simulation.h"
#include "../util/client.h"

class GameSlider : public QSlider {
	Q_OBJECT
public:
	Sim::Simulation *sim;
	QLineEdit *qle;
	bool replay;
	ClientStates *cs;
	GameSlider(Sim::Simulation *sim, ClientStates *cs, QWidget *parent = 0) : QSlider(Qt::Horizontal,parent), cs(cs), sim(sim) {
		replay = false;
		qle = new QLineEdit(parent);
		setRange(0, 100);
		resize(250, height());
		qle->move(width(),0);
		qle->hide();
		setAutoFillBackground(true);
		connect(qle, SIGNAL(textEdited(QString)), this, SLOT(qleChange(QString)));

	}
	void sliderChange(SliderChange sc) {

		if (sc == QAbstractSlider::SliderValueChange) {
			QString str = "%1";
			qle->setText(str.arg(value()));
			cs->setOffset(value());
			update();
			updatePhase();
		}
	}
	void showEvent(QShowEvent *event) {
		qle->show();
	}
	void hideEvent(QHideEvent *event) {
		qle->hide();
	}
	void updatePhase() {
		if (value() == 0) {
			sim->gotoPresent();
		} else {
			sim->gotoPresent();
			if (replay) {
				sim->getReplayManager().rewind(value(), 0);
			} else {
				sim->gotoPresent();
                                sim->getReplayManager().rewind(value()*sim->getConfiguration().stepTime+sim->getCurTime());
			}
		}
	}
public slots:
	void qleChange(const QString text) {
		setValue(text.toInt());
		update();
		updatePhase();
	}
	void remoteUpdate() {
		updatePhase();
	}
};

#endif // GAMESLIDER_H
