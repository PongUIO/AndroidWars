#include<QtGui>
#include<qgl.h>
#include<iostream>
#include "Simulation.h"

class MyGLDrawer : public QGLWidget {
	Q_OBJECT        // must include this if you use Qt signals/slots

public:
        int w, h;
        double cx, cy;
        QImage data[2];
        GLuint texture[2];
        Sim::Simulation *sim;
        Sim::World *wld;
        MyGLDrawer(Sim::Simulation *simIn, QWidget *parent = 0)
			: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
                sim = simIn;
                wld = &sim->getState().getWorld();
                wld->getTile(5,5).mType = 1;
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
                data[0].load(":/graphics/tiles/metal.png");
                texture[0] = bindTexture(data[0]);
                data[1].load(":/graphics/tiles/metal2surf.png");
                texture[1] = bindTexture(data[1]);
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
                int i, j;
                glClear(GL_COLOR_BUFFER_BIT);
                // draw the scene:
                //glRotatef( ... );
                //glMaterialfv( ... );
                glBegin( GL_TRIANGLES );
                glColor3f(0.5,0.5,0.5);
                glVertex2f(-1, -1);
                glVertex2f(1, 1);
                glVertex2f(cx,cy);
                glEnd();
                //glDrawPixels(data.width(), data.height(), GL_RGBA, GL_UNSIGNED_BYTE, gldata.bits());
                glEnable(GL_TEXTURE_2D);

                int mt;

                for (i = 0; i < wld->getHeight(); i++) {
                        for (j = 0; j < wld->getWidth(); j++) {
                                mt = wld->getTile(i, j).mType;
                                glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , data[mt].width(), data[mt].height(),  GL_RGBA, GL_UNSIGNED_BYTE, data[mt].bits() );
                                glBindTexture(GL_TEXTURE_2D, texture[mt]);
                                glBegin(GL_QUADS);
                                glTexCoord2f(0,1); glVertex2f(i/16.-1,j/16.-1);
                                glTexCoord2f(0,0); glVertex2f(i/16.-1,(j+1)/16.-1);
                                glTexCoord2f(1,0); glVertex2f((i+1)/16.-1,(j+1)/16.-1);
                                glTexCoord2f(1,1); glVertex2f((i+1)/16.-1,j/16.-1);
                                glEnd();
                        }
                }
                glDisable(GL_TEXTURE_2D);
                glFlush();
                swapBuffers();
        }

};
