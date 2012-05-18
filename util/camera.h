#ifndef CAMERA_H
#define CAMERA_H
#include "../simulation/Vector.h"
#include<cmath>

#define EDGE 10

class Camera {
public:
	double zoom, dzoom, panFriction, zoomFriction, ratio, scrollSpeed;
	int lastX, lastY, xres, yres;
	Sim::Vector pos, delta, zoomDir;

	Camera(double x, double y, int xres, int yres) {
		pos = Sim::Vector(x,y);
		delta = Sim::Vector(0,0);
		zoomDir = Sim::Vector(0,0);
		dzoom = 0.0;
		zoom = 1;
		calcRatio(xres, yres);
		zoomFriction = 0.9;
		panFriction = 0.9;
		scrollSpeed = 0.002;
        }
        virtual ~Camera() {}

	void calcRatio(double xres, double yres) {
		ratio = yres/((double)xres);
		this->xres = xres;
		this->yres = yres;
        }
        void setLastPos(int lastX, int lastY) {
                this->lastX = lastX;
                this->lastY = lastY;
        }
	void modZoom(double mod) {
		zoomDir =  Sim::Vector(xPixToDouble(lastX), yPixToDouble(lastY));
		dzoom -= mod/10000;
        }

        void iter() {
		if ( 0 < lastX && lastX < xres && 0 < lastY && lastY < yres) {
			delta += Sim::Vector((lastX > xres -EDGE ) * (-(lastX - xres + EDGE)*scrollSpeed) +
				    (lastX < EDGE) * (-(lastX-EDGE)*scrollSpeed),
				    (lastY < EDGE) * ((lastY - EDGE)*scrollSpeed) +
				    ((lastY > yres - EDGE)) * ((lastY - yres + EDGE)* scrollSpeed))/2;
		}
		pos += delta*zoom;
		delta *= panFriction;
		dzoom *= zoomFriction;
		zoom *= dzoom + 1;

                if (zoom < 1) {
                        zoom = 1;
                } else if (zoom > 32) {
                        zoom = 32;
                }

                if (dzoom > 0) {
                        return;
                }
		//qDebug("%4.4f %4.4f\n", temp.x, temp.y);
		pos += zoomDir*(dzoom*5)*(zoom+1);
        }

	double xToSimX(int x) {
		return ((xPixToDouble(x))*(zoom+1)*2-pos.x);
	}

	double yToSimY(int y) {
		return ((yPixToDouble(y))*(zoom+1)*ratio*2-pos.y);
	}
	double xToSimXBack(int x) {
		return ((xPixToDouble(x))*(zoom+2)*2-pos.x);
	}

	double yToSimYBack(int y) {
		return ((yPixToDouble(y))*(zoom+2)*ratio*2-pos.y);
	}

        double xPixToDouble(int x) {
		return ((x*2)/((double)xres)-1);
	}
        double yPixToDouble(int y) {
		return (1-(y*2)/((double)yres));
        }
        int xDoubleToPix(double x) {
		return (-x+1)/xres-pos.x;
        }
	int yDoubleToPix(double y) {
		return (y+1)/yres-pos.y;
        }
	int ySimLim(int i) {
		return yToSimY(i*yres);
	}
	int xSimLim(int i) {
		return xToSimX(i*yres);
	}
};
#endif

