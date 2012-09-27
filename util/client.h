#ifndef CLIENT_H
#define CLIENT_H

#include<boost/unordered_set.hpp>
#include "cursordefines.h"
#include "Simulation.h"
#include "../extsim/ExtSim.h"
#include "../extsim/typerule/program/MoveTowards.h"
#include "../extsim/param/Position.h"
#include<QtGui>


class ClientStates {
private:
	typedef boost::unordered_set<Sim::IdType> idSet;
	idSet mSelBots;
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

	bool isSelected(Sim::IdType b) {
		return mSelBots.find(b) != mSelBots.end();
	}

	int getSelSize() {
		return mSelBots.size();
	}

	void select(Sim::IdType i) {
		if (!mShift) {
			mSelBots.clear();
		}
		mSelBots.insert(i);
	}

	int registerClick(double x, double y, int button) {
		if (mSim == NULL || mGameStepping) {
			return 1;
		}
		if (button == Qt::LeftButton) {
			const std::list<Sim::Bot*> &bots = mSim->getSim().getState().getBotFactory().getBotList();
			std::list<Sim::Bot*>::const_iterator bot = bots.begin();
			if (!mShift) {
				mSelBots.clear();
			}
			for (; bot != bots.end(); bot++) {
				Sim::Vector pos = (*bot)->getBody().mPos;
				Sim::Vector col = (*bot)->getTypePtr()->getCollision()->getBboxHigh();
				if ( pos.x < x && x < pos.x + col.x && pos.y < y && y < pos.y + col.y) {
					qDebug() << (*bot)->getId();
					mSelBots.insert((*bot)->getId());
					return 1;
				}
			}
		} else if (button == Qt::RightButton) {
			mSim->getReplay().gotoActive();
			for (idSet::iterator bot = mSelBots.begin(); bot != mSelBots.end(); bot++) {
				exts::ParamList *paramList = mSim->getData().getProgramDb()
					.getType("MoveTowards")->getRule()->makeParam(0);
				paramList->getParamT<exts::PositionParam>(0)->setVal(Sim::Vector(x, y));


				mSim->getInput().registerInput(paramList);
				mSim->getCpuInput().registerCpuInput(*bot, paramList->getAllocId(0), 0);

			}
			return 1;
		}
		return 0;
	}

};

#endif // CLIENT_H
