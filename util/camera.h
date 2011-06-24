#ifndef CAMERA_H
#define CAMERA_H
#include <QObject>
#include "../simulation/Vector.h"
class Camera : QObject {
        Q_OBJECT
        public slots:
                void iter() {
                        pos += delta;
                        delta *= panfriction;
                        zoom *= dzoom + 2;
                }
        public:
		double zoom, dzoom, panfriction, zoomfriction, ratio;
                Sim::Vector pos, delta;

                Camera(double x, double y, double xres, double yres, QObject *parent = 0) : QObject(parent) {
			pos = Sim::Vector(x,y);
			delta = Sim::Vector(0,0);
                        dzoom = 0.01;
                        zoom = 1./32;
			ratio = yres/xres;
                        panfriction = zoomfriction = 0.9;
		}


                void addVel(double x, double y) {
                        delta += Sim::Vector(x, y);
                }
                void modZoom(double mod) {
                        dzoom += mod;
                }



};
#endif
