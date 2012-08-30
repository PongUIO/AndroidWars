#include "gameslider.h"

GameSlider::GameSlider(exts::ExtSim *sim, ClientStates *cs, QWidget *parent) : QSlider(Qt::Horizontal,parent), mCs(cs), mExtSim(sim) {
	mReplay = true;
	mQle = new QLineEdit(parent);
	setRange(0, 100);
	resize(250, height());
	mQle->move(width(),0);
	mQle->hide();
	setAutoFillBackground(true);
	connect(mQle, SIGNAL(textEdited(QString)), this, SLOT(qleChange(QString)));

}
void GameSlider::sliderChange(SliderChange sc) {

	if (sc == QAbstractSlider::SliderValueChange) {
		QString str = "%1";
		mQle->setText(str.arg(value()));
		mCs->setOffset(value());
		update();
		updatePhase();
	}
}
void GameSlider::showEvent(QShowEvent *event) {
	mQle->show();
}
void GameSlider::hideEvent(QHideEvent *event) {
	mQle->hide();
}
void GameSlider::updatePhase() {
	mExtSim->getReplay().gotoActive();
	if (value() != 0) {
		Sim::Simulation &sim = mExtSim->getSim();
		if (mReplay) {
			int tmp = value() + sim.getCurPhase()*sim.getConfiguration().phaseLength;
			mExtSim->getReplay().replay(tmp/sim.getConfiguration().phaseLength, tmp%sim.getConfiguration().phaseLength);
		} else {
			mExtSim->getReplay().replay(((double)value()*sim.getConfiguration().stepTime+sim.getCurTime()));
		}
	}
}
void GameSlider::qleChange(const QString text) {
	setValue(text.toInt());
	update();
	updatePhase();
}
void GameSlider::remoteUpdate() {
	updatePhase();
}
