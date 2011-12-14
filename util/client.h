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
	public:
		ClientStates() {
		}
		void setSim(Sim::Simulation *in) {
			sim = in;
			selBots.empty();
		}
		bool isSelected(uint b) {
			return selBots.find(b) != selBots.end();
		}
		void registerClick(double x, double y, int button) {
			qDebug() << x << y;
			int i;
			const Sim::BotFactory::ObjVec &bots =  sim->getState().getBotFactory().getBotVector();
			if (sim != NULL) {
				for (i = 0; i < bots.size(); i++) {

					Sim::Bot *bot = bots[i];
					if (bot != NULL) {
						Sim::Vector pos = bot->getBody().mPos;
						qDebug() << pos.x << pos.y;
						if ( pos.x < x && x < pos.x + 1.3 && pos.y < y && y < pos.y + 1.8) {
							selBots.insert(i);
							return;
						}
					}
				}
			}
		}

};

#endif // CLIENT_H
