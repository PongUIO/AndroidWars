#ifndef CAMERA_H
#define CAMERA_H
#include "../simulation/Vector.h"
#include<cmath>

#define EDGE 10

class Camera {
public:
	double mZoom, mDeltaZoom, panFriction, mZoomFriction, mRatio, mScrollSpeed;
	int mLastX, mLastY, mXres, mYres;
	Sim::Vector mPos, mDelta, mZoomDir;

	Camera(double x, double y, int xres, int yres) {
		mPos = Sim::Vector(x,y);
		mDelta = Sim::Vector(0,0);
		mZoomDir = Sim::Vector(0,0);
		mDeltaZoom = 0.0;
		mZoom = 1;
		calcRatio(xres, yres);
		mZoomFriction = 0.9;
		panFriction = 0.9;
		mScrollSpeed = 0.002;
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

        void iter() {
		if ( 0 < mLastX && mLastX < mXres && 0 < mLastY && mLastY < mYres) {
			mDelta += Sim::Vector((mLastX > mXres -EDGE ) * (-(mLastX - mXres + EDGE)*mScrollSpeed) +
				    (mLastX < EDGE) * (-(mLastX-EDGE)*mScrollSpeed),
				    (mLastY < EDGE) * ((mLastY - EDGE)*mScrollSpeed) +
				    ((mLastY > mYres - EDGE)) * ((mLastY - mYres + EDGE)* mScrollSpeed))/2;
		}
		mPos += mDelta*mZoom;
		mDelta *= panFriction;
		mDeltaZoom *= mZoomFriction;
		mZoom *= mDeltaZoom + 1;

		if (mZoom < 1) {
			mZoom = 1;
		} else if (mZoom > 32) {
			mZoom = 32;
                }

		if (mDeltaZoom > 0) {
                        return;
                }
		//qDebug("%4.4f %4.4f\n", temp.x, temp.y);
		mPos += mZoomDir*(mDeltaZoom*5)*(mZoom+1);
        }

	double xToSimX(int x) {
		return ((xPixToDouble(x))*(mZoom+1)-mPos.x);
	}

	double yToSimY(int y) {
		return ((yPixToDouble(y))*(mZoom+1)*mRatio-mPos.y);
	}
	double xToSimXBack(int x) {
		return ((xPixToDouble(x))*(mZoom+2)-mPos.x);
	}

	double yToSimYBack(int y) {
		return ((yPixToDouble(y))*(mZoom+2)*mRatio-mPos.y);
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
		return yToSimY(i*mYres*1.5);
	}
	int xSimLim(int i) {
		return xToSimX(i*mXres*1.4);
	}
};
#endif

