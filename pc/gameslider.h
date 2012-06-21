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
	GameSlider(Sim::Simulation *sim, ClientStates *cs, QWidget *parent = 0);
	void sliderChange(SliderChange sc);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void updatePhase();
public slots:
	void qleChange(const QString text);
	void remoteUpdate();
};

#endif // GAMESLIDER_H
