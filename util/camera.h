#ifndef CAMERA_H
#define CAMERA_H
#include "../simulation/Vector.h"
#include<cmath>
#include<QtGui>

#define EDGE 10
#define MAX_ZOOM 16
#define MIN_ZOOM 1

enum {
	KEY_UP = 0,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_LEFT
};

#define KEY_FIRST KEY_UP
#define KEY_LAST KEY_LEFT

#define KEY_MOVESPEED (1/25.0)

class Camera {
	public:
		double mZoom, mDeltaZoom, mPanFriction, mZoomFriction, mRatio, mScrollSpeed;
		int mLastX, mLastY, mXres, mYres;
		Sim::Vector mPos, mDelta, mZoomDir;
		bool mKeyMovement[4];

		Camera(double x, double y, int xres, int yres) {
			mPos = Sim::Vector(x,y);
			mDelta = Sim::Vector(0,0);
			mZoomDir = Sim::Vector(0,0);
			mDeltaZoom = 0.0;
			mZoom = 1;
			calcRatio(xres, yres);
			mZoomFriction = 0.9;
			mPanFriction = 0.9;
			mScrollSpeed = 0.002;

			for(int i = 0; i < KEY_LAST + 1; i++) {
				mKeyMovement[i] = false;
			}
		}
		virtual ~Camera() {}

		void calcRatio(double xres, double yres) {
			mRatio = yres/((double)xres);
			this->mXres = xres;
			this->mYres = yres;
		}
		void setLastPos(int mLastX, int mLastY) {
			this->mLastX = mLastX;
			this->mLastY = mLastY;
		}
		void modZoom(double mod) {
			mZoomDir =  Sim::Vector(xPixToDouble(mLastX), yPixToDouble(mLastY));
			mDeltaZoom -= mod/10000;
		}

		void setKeyMoveState(int dir, bool state) {
			if (dir < KEY_FIRST || KEY_LAST < dir )
				return;
			mKeyMovement[dir] = state;
		}

		void iter() {
			mDelta.y -= mKeyMovement[KEY_UP] * KEY_MOVESPEED;
			mDelta.y += mKeyMovement[KEY_DOWN] * KEY_MOVESPEED;
			mDelta.x -= mKeyMovement[KEY_RIGHT] * KEY_MOVESPEED;
			mDelta.x += mKeyMovement[KEY_LEFT] * KEY_MOVESPEED;

			if ( 0 < mLastX && mLastX < mXres && 0 < mLastY && mLastY < mYres) {
				mDelta += Sim::Vector((mLastX > mXres -EDGE ) * (-(mLastX - mXres + EDGE)*mScrollSpeed) +
						(mLastX < EDGE) * (-(mLastX-EDGE)*mScrollSpeed),
						(mLastY < EDGE) * ((mLastY - EDGE)*mScrollSpeed) +
						((mLastY > mYres - EDGE)) * ((mLastY - mYres + EDGE)* mScrollSpeed))/2;
			}
			mPos += mDelta*mZoom;
			mDelta *= mPanFriction;
			mDeltaZoom *= mZoomFriction;
			mZoom *= mDeltaZoom + 1;

			if (mZoom < MIN_ZOOM) {
				mZoom = MIN_ZOOM;
			} else if (mZoom > MAX_ZOOM) {
				mZoom = MAX_ZOOM;
			}

			if (mDeltaZoom > 0) {
				return;
			}
			//qDebug("%4.4f %4.4f\n", temp.x, temp.y);
			mPos += mZoomDir*(mDeltaZoom*5)*(mZoom+1);
		}

		double xToSimLocal(int x, double zoom) {
			return ((xPixToDouble(x))*zoom);
		}

		double yToSimLocal(int y, double zoom) {
			return ((yPixToDouble(y))*zoom*mRatio);
		}
		double xToSim(int x) {
			return (xToSimLocal(x, mZoom + 1) - mPos.x);
		}

		double yToSim(int y) {
			return (yToSimLocal(y, mZoom + 1) - mPos.y);
		}
		double xToSimBack(int x) {
			return (xToSimLocal(x, mZoom + 2) - mPos.x);
		}

		double yToSimBack(int y) {
			return (yToSimLocal(y, mZoom + 2) - mPos.y);
		}

		double xPixToDouble(int x) {
			return ((x*2)/((double)mXres)-1);
		}
		double yPixToDouble(int y) {
			return (1-(y*2)/((double)mYres));
		}
		int xDoubleToPix(double x) {
			return (-x+1)/mXres-mPos.x;
		}
		int yDoubleToPix(double y) {
			return (y+1)/mYres-mPos.y;
		}
		int ySimLim(int i) {
			return yToSim(i*mYres*1.5);
		}
		int xSimLim(int i) {
			return xToSim(i*mXres*1.4);
		}
};
#endif

