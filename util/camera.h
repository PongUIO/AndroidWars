#ifndef CAMERA_H
#define CAMERA_H
#include "../simulation/Vector.h"
#include<cmath>
#include<QtGui>

#define DELTAMINLIMIT 0.08
#define ZOOMSPEED 0.07
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
		double mZoom, mDeltaZoom, mPanFriction, mZoomFriction, mRatio, mScrollSpeed, mZoomDistRest;
		int mLastX, mLastY, mXres, mYres;
		Sim::Vector mPos, mDelta, mZoomTarget;
		bool mKeyMovement[4], mDragMove;

		Camera(double x, double y, int xres, int yres) {
			mPos = Sim::Vector(x,y);
			mDelta = Sim::Vector(0,0);
			mZoomTarget = Sim::Vector(0,0);
			mDeltaZoom = 0.0;
			mZoom = 1;
			calcRatio(xres, yres);
			mZoomFriction = 0.9;
			mPanFriction = 0.9;
			mScrollSpeed = 0.002;
			mDragMove = false;
			mZoomDistRest = 0;

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

		void setLastPos(int nx, int ny) {
			handleMouseMove(nx, ny);
			this->mLastX = nx;
			this->mLastY = ny;
		}

		void modZoom(double mod) {
			mZoomTarget =  Sim::Vector(xPixToDouble(mLastX), yPixToDouble(mLastY));
			mZoomDistRest = Sim::Vector(xToSimLocal(mLastX, mZoom+0.5), yToSimLocal(mLastY, mZoom+0.5)).len();
			mDeltaZoom = 0.05*((mod > 0) * -2 + 1);
		}

		void setKeyMoveState(int dir, bool state) {
			if (dir < KEY_FIRST || KEY_LAST < dir )
				return;
			mKeyMovement[dir] = state;
		}

		void setDragMove(bool state) {
			mDragMove = state;
		}

		void handleMouseMove(int nx, int ny) {
			if (mDragMove) {
				mPos.x += xToSimLocal(nx, mZoom + 0.5) - xToSimLocal(mLastX, mZoom+0.5);
				mPos.y += yToSimLocal(ny, mZoom + 0.5) - yToSimLocal(mLastY, mZoom+0.5);
			}
		}

		void stopZoom() {
			mZoomDistRest = 0;
			mDeltaZoom = 0;
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
			if (mDeltaZoom > 0) {
				mZoom *= mDeltaZoom + 1;
				mDeltaZoom *= mZoomFriction;
				mZoomDistRest = 0;
			}

			//qDebug("%4.4f %4.4f\n", temp.x, temp.y);
			if (mZoomDistRest > 0.01) {
				Sim::Vector delta = mZoomTarget*(mDeltaZoom*10);
				if (delta.len() < DELTAMINLIMIT) {
					delta = delta.normalize()*DELTAMINLIMIT;
				}
				mPos += delta;
				mZoomDistRest -= delta.len();
				mZoom *= mDeltaZoom + 1;
			}
			if (mZoom < MIN_ZOOM) {
				mZoom = MIN_ZOOM;
			} else if (mZoom > MAX_ZOOM) {
				mZoom = MAX_ZOOM;
			}

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

