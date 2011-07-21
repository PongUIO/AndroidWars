#ifndef CAMERA_H
#define CAMERA_H
#include "../simulation/Vector.h"
#include<cmath>
class Camera {
public:
        double zoom, dzoom, panfriction, zoomfriction, ratio;
        int lastX, lastY, xres, yres;
        Sim::Vector pos, delta;

        Camera(double x, double y, int xres, int yres) {
                pos = Sim::Vector(x,y);
                delta = Sim::Vector(-0.005,-0.005);
                dzoom = 0.001;
                zoom = 1;
                ratio = yres/((double)xres);
                this->xres = xres;
                this->yres = yres;
                zoomfriction = 0.99;
                panfriction = 0.9;
        }
        virtual ~Camera() {}


        void addVel(int lastX, int lastY) {
                this->lastX = lastX;
                this->lastY = lastY;
                delta += Sim::Vector((lastX > xres -10 ) * (-(lastX - xres + 10)*0.00001) +
                            (lastX < 10) * (-(lastX-10)*0.00001),
                            (lastY < 10) * ((lastY - 10)*0.00001) +
                            (lastY > yres - 10) * ((lastY - yres + 10)* 0.00001))/2;
        }
        void modZoom(double mod) {
                dzoom += mod;
        }

        void iter() {
                pos += delta*zoom;
                delta *= panfriction;
                zoom *= dzoom + 1;
                dzoom *= zoomfriction;

                Sim::Vector temp = Sim::Vector( xPixToDouble(lastX), yPixToDouble(lastY))*(-fabs(dzoom*5));
                //qDebug("%4.4f %4.4f\n", temp.x, temp.y);
                pos += temp;
                if (zoom < 0.01) {
                        zoom = 0.01;
                } else if (zoom > 32) {
                        zoom = 32;
                }
        }

        double xPixToDouble(int x) {
                return ((x*2)/((double)xres)-1);
        }
        double yPixToDouble(int y) {
                return (1-(y*2)/((double)yres));
        }
        int xDoubleToPix(double x) {
                return (-x+1)/xres;
        }
        int yDoubleToPix(double y) {
                return (y+1)/yres;
        }
};
#endif

