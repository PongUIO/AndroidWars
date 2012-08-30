#ifndef CLIENT_H
#define CLIENT_H

#include<boost/unordered_set.hpp>
#include "cursordefines.h"
#include "Simulation.h"
#include "../extsim/ExtSim.h"
#include "../extsim/typerule/program/MoveTowards.h"
#include "../extsim/param/Position.h"

class ClientStates {
private:
	boost::unordered_set<uint> mSelBots;
	exts::ExtSim *mSim;
	bool mShift, mCtrl, mMenu, mGameStepping;
	int mOffset;
public:
	ClientStates() {
		mOffset = 0;
		mShift = mCtrl = mGameStepping = false;
		mMenu = true;
		mSim = NULL;
	}

	exts::ExtSim* getSim() {
		return mSim;
	}

	void setOffset(int i) {
		mOffset = i;
	}

	void setExtSim(exts::ExtSim *in) {
		mShift = mCtrl = mMenu = false;
		mSim = in;
		mSelBots.clear();
	}
	void releaseMods() {
		mShift = false;
		mCtrl = false;
	}
	void setShift(bool state) {
		mShift = state;
	}

	void setCtrl(bool state) {
		mCtrl = state;
	}
	void setMenu(bool state) {
		mMenu = state;
	}
	void setRunning(bool state) {
		mGameStepping = state;
	}
	bool getRunning() {
		return mGameStepping;
	}
	bool menuOpen() {
		return mMenu;
	}

	bool isSelected(uint b) {
		return mSelBots.find(b) != mSelBots.end();
	}

	int getSelSize() {
		return mSelBots.size();
	}

	void select(uint i) {
		if (!mShift) {
			mSelBots.clear();
		}
		mSelBots.insert(i);
	}

	int registerClick(double x, double y, int button) {
		if (mSim == NULL || mGameStepping) {
			return 1;
		}
		std::list<Sim::Bot*> bots = mSim->getSim().getState().getBotFactory().getBotList();
		std::list<Sim::Bot*>::iterator bot;
		if (button) {
			if (!mShift) {
				mSelBots.clear();
			}
			for (bot = bots.begin(); bot != bots.end(); bot++) {
				Sim::Vector pos = (*bot)->getBody().mPos;
				Sim::Vector col = (*bot)->getTypePtr()->getCollision()->getBboxHigh();
				if ( pos.x < x && x < pos.x + col.x && pos.y < y && y < pos.y + col.y) {
					mSelBots.insert((*bot)->getId());
					return 1;
				}
			}
		} else if (button == 0) {
			for (bot = bots.begin(); bot != bots.end(); bot++) {
				if (isSelected((*bot)->getId())) {
					mSim->getReplay().gotoActive();
					exts::ParamList *paramList = mSim->getData().getProgramDb()
						.getType("MoveTowards")->getRule()->makeParam(0);
					paramList->getParamT<exts::PositionParam>(0)->setVal(Sim::Vector(x, y));


					mSim->getInput().registerInput(paramList);
					mSim->getCpuInput().registerCpuInput((*bot)->getId(), paramList->getAllocId(0), 0);

				}
			}
			return 1;
		}
		return 0;
	}

};

#endif // CLIENT_H
