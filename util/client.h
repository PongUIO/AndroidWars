#ifndef CLIENT_H
#define CLIENT_H

#include<boost/unordered_set.hpp>
#include "cursordefines.h"
#include "Simulation.h"

class ClientStates {
private:
	boost::unordered_set<uint> selBots;
	Sim::Simulation *sim;
	bool shift, ctrl, menu, gameStepping;
	int offset;
public:
	ClientStates() {
		offset = 0;
		shift = ctrl = gameStepping = false;
		menu = true;
		sim = NULL;
	}

	Sim::Simulation* getSim() {
		return sim;
	}

	void setOffset(int i) {
		offset = i;
	}

	void setSim(Sim::Simulation *in) {
		shift = ctrl = menu = false;
		sim = in;
		selBots.clear();
	}
	void releaseMods() {
		shift = false;
		ctrl = false;
	}
	void setShift(bool state) {
		shift = state;
	}

	void setCtrl(bool state) {
		ctrl = state;
	}
	void setMenu(bool state) {
		menu = state;
	}
	void setRunning(bool state) {
		gameStepping = state;
	}
	bool getRunning() {
		return gameStepping;
	}
	bool menuOpen() {
		return menu;
	}

	bool isSelected(uint b) {
		return selBots.find(b) != selBots.end();
	}

	int getSelSize() {
		return selBots.size();
	}

	void select(uint i) {
		if (!shift) {
			selBots.clear();
		}
		selBots.insert(i);
	}

	int registerClick(double x, double y, int button) {
		if (sim == NULL || gameStepping) {
			return 1;
		}
		std::list<Sim::Bot*> bots = sim->getState().getBotFactory().getBotList();
		std::list<Sim::Bot*>::iterator bot;
		if (button) {
			if (!shift) {
				selBots.clear();
			}
			for (bot = bots.begin(); bot != bots.end(); bot++) {
				Sim::Vector pos = (*bot)->getBody().mPos;
				Sim::Vector col = (*bot)->getTypePtr()->getCollision()->getBboxHigh();
				if ( pos.x < x && x < pos.x + col.x && pos.y < y && y < pos.y + col.y) {
					selBots.insert((*bot)->getId());
					return 1;
				}
			}
		} else if (button == 0) {
			for (bot = bots.begin(); bot != bots.end(); bot++) {
				if (isSelected((*bot)->getId())) {
					//sim->gotoPresent();
					//sim->getReplayManager().loadCurPhaseInput();
					Sim::Input &inMgr = sim->getInput();
					Sim::ProgramFactory &progFact = sim->getState().getProgramFactory();

					using namespace Sim::Prog;
					MoveTowards *move = inMgr.getProgramInput().buildInputImplStr<MoveTowards>(
						    MoveTowards::Config(MoveTowards::DtPosition, Sim::Vector(x, y)), "MoveTowards"
						    );
					//Kill *kill = inMgr.getProgramInput().buildInputImpl<Kill>(
					//Kill::Config(move->getId()));
					inMgr.getCpuInput().registerInput((*bot)->getId(), move->getId(), offset);
					//inMgr.getCpuInput().registerInput((*bot)->getId(), kill->getId(), 20);
				}
			}
			return 1;
		}
		return 0;
	}

};

#endif // CLIENT_H
