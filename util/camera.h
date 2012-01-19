#ifndef CAMERA_H
#define CAMERA_H
#include "../simulation/Vector.h"
#include<cmath>

#define EDGE 10

class Camera {
public:
        double zoom, dzoom, panfriction, zoomfriction, ratio;
        int lastX, lastY, xres, yres;
        Sim::Vector pos, delta;

        Camera(double x, double y, int xres, int yres) {
                pos = Sim::Vector(x,y);
                delta = Sim::Vector(-0.005,-0.005);
                dzoom = 0.0;
		zoom = 1;
		calcRatio(xres, yres);
		zoomfriction = 0.9;
                panfriction = 0.9;
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
		dzoom -= mod/10000;
        }

        void iter() {
                qDebug() << lastX << lastY;
                if ( 0 < lastX && lastX < xres && 0 < lastY && lastY < yres) {
                        delta += Sim::Vector((lastX > xres -EDGE ) * (-(lastX - xres + EDGE)*0.001) +
                                    (lastX < EDGE) * (-(lastX-EDGE)*0.001),
                                    (lastY < EDGE) * ((lastY - EDGE)*0.001) +
                                    ((lastY > yres - EDGE)) * ((lastY - yres + EDGE)* 0.001))/2;
                }
                pos += delta*zoom;
                delta *= panfriction;
                dzoom *= zoomfriction;
		zoom *= dzoom + 1;

                if (zoom < 1) {
                        zoom = 1;
                } else if (zoom > 32) {
                        zoom = 32;
                }

                if (dzoom > 0) {
                        return;
                }
                Sim::Vector temp = Sim::Vector( xPixToDouble(lastX), yPixToDouble(lastY))*(-fabs(dzoom*5));
                //qDebug("%4.4f %4.4f\n", temp.x, temp.y);
                pos += temp;
        }	

	double xToSimX(int x) {
		return ((xPixToDouble(x))*zoom-pos.x);
	}

	double yToSimY(int y) {
		return ((yPixToDouble(y))*zoom*ratio-pos.y);
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
};
#endif

