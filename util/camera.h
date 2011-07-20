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
                zoomfriction = 0.99;
                panfriction = 0.9;
        }
        virtual ~Camera() {}


        void addVel(double x, double y) {
                delta += Sim::Vector(x, y)/2;
        }
        void modZoom(double mod) {
                dzoom += mod;
        }

        void iter() {
                pos += delta*zoom;
                delta *= panfriction;
                zoom *= dzoom + 1;
                dzoom *= zoomfriction;
                if (zoom < 0.01) {
                        zoom = 0.01;
                } else if (zoom > 32) {
                        zoom = 32;
                }
        }


};
#endif

