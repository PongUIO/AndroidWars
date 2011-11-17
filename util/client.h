#ifndef CLIENT_H
#define CLIENT_H

#include<boost/unordered_set.hpp>
#include "cursordefines.h"
#include<QDebug>

class ClientStates {
	private:
		boost::unordered_set<uint> bots;
	public:
		ClientStates() {
		}
		bool isSelected(uint b) {
			return bots.find(b) != bots.end();
		}
		void registerClick(int x, int y, int button) {
		}


};

#endif // CLIENT_H
