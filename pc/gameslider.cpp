#include "gameslider.h"

GameSlider::GameSlider(Sim::Simulation *sim, ClientStates *cs, QWidget *parent) : QSlider(Qt::Horizontal,parent), cs(cs), sim(sim) {
	replay = true;
	qle = new QLineEdit(parent);
	setRange(0, 100);
	resize(250, height());
	qle->move(width(),0);
	qle->hide();
	setAutoFillBackground(true);
	connect(qle, SIGNAL(textEdited(QString)), this, SLOT(qleChange(QString)));

}
void GameSlider::sliderChange(SliderChange sc) {

	if (sc == QAbstractSlider::SliderValueChange) {
		QString str = "%1";
		qle->setText(str.arg(value()));
		cs->setOffset(value());
		update();
		updatePhase();
	}
}
void GameSlider::showEvent(QShowEvent *event) {
	qle->show();
}
void GameSlider::hideEvent(QHideEvent *event) {
	qle->hide();
}
void GameSlider::updatePhase() {
	/*sim->gotoPresent();
	if (value() != 0) {
		if (replay) {
			int tmp = value() + sim->getCurPhase()*sim->getConfiguration().phaseLength;
			sim->getReplayManager().rewind(tmp/sim->getConfiguration().phaseLength, tmp%sim->getConfiguration().phaseLength);
		} else {
			sim->getReplayManager().rewind(((double)value()*sim->getConfiguration().stepTime+sim->getCurTime()));
		}
	}*/
}
void GameSlider::qleChange(const QString text) {
	setValue(text.toInt());
	update();
	updatePhase();
}
void GameSlider::remoteUpdate() {
	updatePhase();
}
