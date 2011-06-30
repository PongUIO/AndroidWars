#ifndef CAMERA_H
#define CAMERA_H
#include "../simulation/Vector.h"
class Camera {
public:
        double zoom, dzoom, panfriction, zoomfriction, ratio;
        Sim::Vector pos, delta;

        Camera(double x, double y, double xres, double yres) {
                pos = Sim::Vector(x,y);
                delta = Sim::Vector(-0.005,-0.005);
                dzoom = 0.001;
                zoom = 1;
                ratio = yres/xres;
                panfriction = zoomfriction = 0.9;
        }
        virtual ~Camera() {}


        void addVel(double x, double y) {
                delta += Sim::Vector(x, y);
        }
        void modZoom(double mod) {
                dzoom += mod;
        }

        void iter() {
                pos += delta;
                //delta *= panfriction;
                zoom *= dzoom + 1;
        }


};
#endif

