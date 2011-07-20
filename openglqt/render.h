#ifndef RENDER_H
#define RENDER_H
#include<QtGui>
#include<qgl.h>
#include<iostream>
#include "Simulation.h"
#include "../util/camera.h"

class MyGLDrawer : public QGLWidget {
	Q_OBJECT        // must include this if you use Qt signals/slots

public slots:
        void redraw() {
                paintGL();
        }

        void moveMouseCheck() {

                cam->addVel((lastX > width() -10 ) * (-(lastX - width() + 10)*0.00001) +
                            (lastX < 10) * (-(lastX-10)*0.00001),
                            (lastY < 10) * ((lastY - 10)*0.00001) +
                            (lastY > height() - 10) * ((lastY - height() + 10)* 0.00001));
        }

public:
        int lastX, lastY;
	QImage data[3];
	GLuint texture[3];
        QImage characters[1];
        GLuint chartexture[1];
        Sim::Simulation *sim;
        Sim::World *wld;
        Camera *cam;
        MyGLDrawer(Camera *cam, Sim::Simulation *simIn, QWidget *parent = 0)
		: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
                this->cam = cam;
                lastX = width()/2;
                lastY = height()/2;
                sim = simIn;
                wld = &sim->getState().getWorld();
		wld->getTile(3,0).setType(1);


        }

protected:
        // overridden
        void keyPressEvent (QKeyEvent *event) {
		//              qDebug() << event->key();
        }

        // overriden
        void mouseMoveEvent(QMouseEvent * event) {
                lastX = event->pos().x();
                lastY = event->pos().y();
		//                qDebug() << event->pos().x() << " " << event->pos().y();
        }

        // overridden  
        void mousePressEvent(QMouseEvent * event) {
                int w = width();
                int h = height();
        }



	// overridden
        void initializeGL()
        {
                // Set up the rendering context, define display lists etc.:
                glClearColor( 0.0, 0.0, 0.0, 0.0 );
		glEnable(GL_DEPTH_TEST | GL_DOUBLE);
		data[0].load(":/graphics/tiles/empty.png");
		texture[0] = bindTexture(data[0].scaled(64,64));
		data[1].load(":/graphics/tiles/metal.png");
		texture[1] = bindTexture(data[1].scaled(64,64));
		data[2].load(":/graphics/tiles/metal2surf.png");
		texture[2] = bindTexture(data[2].scaled(64,64));
                characters[0].load(":/graphics/characters/temp.png");
                chartexture[0] = bindTexture(characters[0].scaled(128,320));

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

        void wheelEvent(QWheelEvent *event) {
                cam->dzoom -= event->delta()/500000.0;//pow(10, cam->zoom);
        }

        // overridden
        void paintGL()
        {
                moveMouseCheck();
                cam->iter();
                int i, j;
                glClear(GL_COLOR_BUFFER_BIT);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(-cam->zoom, cam->zoom, -cam->zoom*cam->ratio, cam->zoom*cam->ratio, 0.01, 1000);
                glTranslatef(cam->pos.x,cam->pos.y,-1);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                // draw the scene:
                //glRotatef( ... );
                //glMaterialfv( ... );
                //glDrawPixels(data.width(), data.height(), GL_RGBA, GL_UNSIGNED_BYTE, gldata.bits());
                glEnable(GL_TEXTURE_2D);

                int mt;
                //       glViewport(cam->pos.x, cam->pos.y, getWidth(), getHeight());
                int fx = -cam->pos.x-cam->zoom-1;
                int tx = -cam->pos.x+cam->zoom+1;
                int fy = -cam->pos.y-cam->zoom*cam->ratio-1;
                int ty = -cam->pos.y+cam->zoom*cam->ratio+1;
                for (i = fx; i < tx; i++) {
                        for (j = fy; j < ty; j++) {
                                mt = wld->getTile(i, j).getType();
                                if (mt == 0) {
                                        continue;
                                }
                                glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , data[mt].width(), data[mt].height(),  GL_RGBA, GL_UNSIGNED_BYTE, data[mt].bits() );
                                glBindTexture(GL_TEXTURE_2D, texture[mt]);
                                glBegin(GL_QUADS);
                                glTexCoord2f(0,1); glVertex2f(i,j+1);  // lower left
                                glTexCoord2f(0,0); glVertex2f(i,j); // lower right
                                glTexCoord2f(1,0); glVertex2f(i+1,j);// upper right
                                glTexCoord2f(1,1); glVertex2f(i+1,j+1); // upper left
                                glEnd();
                        }

                }

                const Sim::BotFactory::ObjVec &bots =  sim->getState().getBotFactory().getBotVector();

                for (i = 0; i < bots.size(); i++) {
                        Sim::Bot *bot = bots[i];
                        if (bot != NULL) {
                                Sim::Vector vec = bot->getBody().mPos;
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , characters[mt].width(), characters[mt].height(),  GL_RGBA, GL_UNSIGNED_BYTE, characters[mt].bits() );
				glBindTexture(GL_TEXTURE_2D, chartexture[0]);
                                glBegin(GL_QUADS);
                                glTexCoord2f(0,1); glVertex2f(vec.x,vec.y + 1.8);  // lower left
                                glTexCoord2f(0,0); glVertex2f(vec.x,vec.y); // lower right
                                glTexCoord2f(1,0); glVertex2f(vec.x+1,vec.y);// upper right
                                glTexCoord2f(1,1); glVertex2f(vec.x+1,vec.y+1.8); // upper left
                                glEnd();
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glBindTexture(GL_TEXTURE_2D, 0);
				glBlendFunc(GL_ONE, GL_ONE);
				//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

				glBegin(GL_QUADS);
				glColor3f(0.0,1.0,1.0);
				glVertex2f(vec.x,vec.y + 1.8);
				glVertex2f(vec.x,vec.y);
				glVertex2f(vec.x+1,vec.y);
				glVertex2f(vec.x+1,vec.y+1.8);
				glEnd();
				glBlendFunc(GL_ONE, GL_ZERO);
				glDisable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
				glColor3f(1.0,1.0,1.0);

                        }
                }
                glDisable(GL_TEXTURE_2D);
		glFlush();
		glFinish();
                swapBuffers();
        }
        int getXBlock(double x) {
                return 0;
        }

        double getXPix(int x) {
                return -1+(((double)x)/width())*2-cam->pos.x;
        }
        double getYPix(int y) {
                return 1-(((double)y)/height())*2-cam->pos.y;
        }

};
#endif
