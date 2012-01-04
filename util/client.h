#ifndef CLIENT_H
#define CLIENT_H

#include<boost/unordered_set.hpp>
#include "cursordefines.h"
#include "Simulation.h"
#include<QDebug>

class ClientStates {
	private:
		boost::unordered_set<uint> selBots;
		Sim::Simulation *sim;
		bool shift, ctrl;
	public:
		ClientStates() {
		}

		Sim::Simulation* getSim() {
			return sim;
		}

		void setSim(Sim::Simulation *in) {
			shift = false;
			ctrl = false;
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

		bool isSelected(uint b) {
			return selBots.find(b) != selBots.end();
		}

		void select(uint i) {
			if (!shift) {
				selBots.clear();
			}
			selBots.insert(i);
		}

		void registerClick(double x, double y, int button) {
			int i;
			if (sim != NULL) {
				std::list<Sim::Bot*> bots = sim->getState().getBotFactory().getBotList();
				std::list<Sim::Bot*>::iterator bot;
				if (!shift) {
					selBots.clear();
				}
				for (bot = bots.begin(); bot != bots.end(); bot++) {
					Sim::Vector pos = (*bot)->getBody().mPos;
					Sim::Vector col = (*bot)->getTypePtr()->getCollision()->getBboxHigh();
					if ( pos.x < x && x < pos.x + col.x && pos.y < y && y < pos.y + col.y) {
						selBots.insert((*bot)->getId());
						return;
					}
				}
			}
		}

};

#endif // CLIENT_H
