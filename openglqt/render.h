#include<QtGui>
#include<qgl.h>
#include<iostream>
#include "Simulation.h"

class MyGLDrawer : public QGLWidget {
	Q_OBJECT        // must include this if you use Qt signals/slots

public:
        int w, h;
        double cx, cy;
        QImage data;
        GLuint texture[1];
        Sim::Simulation *sim;
        MyGLDrawer(Sim::Simulation *simIn, QWidget *parent = 0)
			: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
                sim = simIn;
                cx = 1; // Current x
                cy = 1; // Current y

        }

protected:
        // overridden
        void keyPressEvent (QKeyEvent *event) {
                std::cout << event->key() << std::endl;
        }

        // overridden
        void mousePressEvent(QMouseEvent * event) {
                w = width();
                h = height();
                cx = -1+(((double)event->x())/width())*2;
                cy = 1-(((double)event->y())/height())*2;
                std::cout << cx << " " << cy << std::endl << width() << " " << height() << std::endl;
                paintGL();
        }



        // overridden
        void initializeGL()
        {
                // Set up the rendering context, define display lists etc.:
                glClearColor( 0.0, 0.0, 0.0, 0.0 );
                glEnable(GL_DEPTH_TEST | GL_DOUBLE);
                data.load(":/graphics/tiles/metal.png");
                texture[0] = bindTexture(data);
/*		glEnable(GL_TEXTURE_2D);
                glGenTextures(3,&texture[0]);
                glBindTexture(GL_TEXTURE_2D,texture[0]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glBindTexture(GL_TEXTURE_2D, texture[0]);
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, gldata.width(),
                              gldata.height(),
                              0, GL_RGBA, GL_UNSIGNED_BYTE, gldata.bits() );

                glDisable(GL_TEXTURE_2D);*/

        }

        // overridden
        void resizeGL( int w, int h )
        {
                // setup viewport, projection etc.:
                glViewport( 0, 0, (GLint)w, (GLint)h );
                //glFrustum( ... );
        }

        // overridden
        void paintGL()
        {
                glClear(GL_COLOR_BUFFER_BIT);
                // draw the scene:
                //glRotatef( ... );
                //glMaterialfv( ... );
                glBegin( GL_TRIANGLES );
                glColor3f(0.7,0.7,0.7);
                glVertex2f(-1, -1);
                glVertex2f(1, 1);
                glVertex2f(cx,cy);
                glEnd();
                //glDrawPixels(data.width(), data.height(), GL_RGBA, GL_UNSIGNED_BYTE, gldata.bits());
                glEnable(GL_TEXTURE_2D);

                glBindTexture(GL_TEXTURE_2D, texture[0]); // Actually have an array of images

                glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , data.width(), data.height(),  GL_RGBA, GL_UNSIGNED_BYTE, data.bits() );

                glBegin(GL_QUADS);   // in theory triangles are better
                glTexCoord2f(0,0); glVertex2f(0,0.5);
                glTexCoord2f(0,1); glVertex2f(0,0);
                glTexCoord2f(1,1); glVertex2f(0.5,0);
                glTexCoord2f(1,0); glVertex2f(0.5,0.5);
                glEnd();
                glDisable(GL_TEXTURE_2D);
                glFlush();
                swapBuffers();
        }

};
