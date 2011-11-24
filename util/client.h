#ifndef CLIENT_H
#define CLIENT_H

#include<boost/unordered_set.hpp>
#include "cursordefines.h"
#include "Simulation.h"
#include<QDebug>

class ClientStates {
	private:
		boost::unordered_set<uint> bots;
		Sim::Simulation *sim;
	public:
		ClientStates() {
		}
		void setDrawer(Sim::Simulation *in) {
			sim = in;
			bots.empty();
		}
		bool isSelected(uint b) {
			return bots.find(b) != bots.end();
		}
		void registerClick(double x, double y, int button) {
			int i;
			if (sim != NULL) {
				std::vector<Sim::Bot*> bots = sim->getState().getBotFactory().getBotVector();
				for (i = 0; i < bots.size(); i++) {
					bots.at(i);

				}
			}
		}

};

#endif // CLIENT_H
