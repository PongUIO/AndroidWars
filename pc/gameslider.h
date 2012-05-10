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
		replay = true;
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
		sim->gotoPresent();
		if (value() != 0) {
			if (replay) {
				int tmp = value() + sim->getCurPhase()*sim->getConfiguration().phaseLength;
				sim->getReplayManager().rewind(tmp/sim->getConfiguration().phaseLength, tmp%sim->getConfiguration().phaseLength);
			} else {
				sim->getReplayManager().rewind(((double)value()*sim->getConfiguration().stepTime+sim->getCurTime()));
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
