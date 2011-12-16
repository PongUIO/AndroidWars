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
		void setSim(Sim::Simulation *in) {
			shift = false;
			ctrl = false;
			sim = in;
			selBots.clear();
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

		void registerClick(double x, double y, int button) {
			int i;
			const Sim::BotFactory::ObjVec &bots =  sim->getState().getBotFactory().getBotVector();
			if (sim != NULL) {
				if (!shift) {
					selBots.clear();
				}
				for (i = 0; i < bots.size(); i++) {
					Sim::Bot *bot = bots[i];
					if (bot != NULL) {
						Sim::Vector pos = bot->getBody().mPos;
						Sim::Vector col = bot->getTypePtr()->getCollision()->getBboxHigh();
						qDebug() << pos.x << pos.y;
						if ( pos.x < x && x < pos.x + col.x && pos.y < y && y < pos.y + col.y) {
							selBots.insert(i);
							return;
						}
					}
				}
			}
		}

};

#endif // CLIENT_H
