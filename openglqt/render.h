#ifndef RENDER_H
#define RENDER_H
#include<QtGui>
#include<qgl.h>
#include<iostream>
#include "Simulation.h"
#include "../util/camera.h"
#include "../util/client.h"
#include "../util/cursordefines.h"

class MyGLDrawer : public QGLWidget {
	Q_OBJECT        // must include this if you use Qt signals/slots

public slots:
        void redraw() {
                paintGL();
        }

        void moveMouseCheck() {

                cam->addVel(lastX, lastY);
        }

public:
        int lastX, lastY;
	QImage data[3];
        GLuint textures[3];
        QImage characters[1];
        GLuint chartextures[1];
        QImage weapons[1];
        GLuint weaponstextures[1];
        QImage bg[1];
        GLuint bgtextures[1];
	QImage mouse[2];
	GLuint mousetextures[2];
        QPixmap m;
        Sim::Simulation *sim;
        Sim::World *wld;
	Camera *cam;
	ClientStates *states;
	int cMouse;
	double mouseSize;
	MyGLDrawer(Camera *cam, Sim::Simulation *simIn, ClientStates *states, QWidget *parent = 0)
		: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
		cMouse = 0;
                this->cam = cam;
                lastX = width()/2;
                lastY = height()/2;
                sim = simIn;
		wld = &sim->getState().getWorld();
		this->states = states;

		setCursor( QCursor( Qt::BlankCursor ) );
		mouseSize = 0.07;
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
		// we need to ask the world what we're pointing at!
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
                //glEnable(GL_DEPTH_TEST);
		glEnable(GL_DOUBLE);

		//Loading textures.
		loadAndBind(":/graphics/tiles/empty.png", &data[0], &textures[0]);
		loadAndBind(":/graphics/tiles/metal.png", &data[1], &textures[1]);
		loadAndBind(":/graphics/tiles/metal2surf.png", &data[2], &textures[2]);
		loadAndBind(":/graphics/characters/character1.png", &characters[0], &chartextures[0], 128, 320, false, true);
		loadAndBind(":/graphics/weapons/testweapon.png", &weapons[0], &weaponstextures[0], 32, 64);
		loadAndBind(":/graphics/mouse/default.png", &mouse[0], &mousetextures[0],16,16);
		loadAndBind(":/graphics/mouse/attack.png", &mouse[1], &mousetextures[1],64,64);
		this->setAttribute(Qt::WA_NoSystemBackground);
		QPixmap m;
		m.convertFromImage(mouse[MOUSE_ATTACK]);
		this->setCursor(QCursor(m, -1, -1));

        }

	void loadAndBind(const char *path, QImage *img, GLuint *bind, GLuint xsize = -1, GLuint ysize = -1, bool vertFlip = false, bool horFlip = false) {
		if (xsize != -1) {
			img->load(path);
			*img = (*img).scaled(xsize, ysize).mirrored(horFlip, vertFlip);
		} else {
			img->load(path);
		}
		*bind = bindTexture(*img);
	}

        // overridden
        void resizeGL( int w, int h )
        {
                // setup viewport, projection etc.:
		glViewport( 0, 0, (GLint)w, (GLint)h );
		cam->calcRatio(w, h);
	}

	void resizeEvent(QResizeEvent *event) {
		qDebug() << "resize!!";
		resizeGL(event->size().width(), event->size().height());
		resize(event->size());
	}

        void wheelEvent(QWheelEvent *event) {
		cam->modZoom(event->delta());
        }

        // overridden
        void paintGL()
        {
                cam->iter();
                int i, j;
                glClear(GL_COLOR_BUFFER_BIT);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(-cam->zoom, cam->zoom, -cam->zoom*cam->ratio, cam->zoom*cam->ratio, 0.01, 1000);
                glTranslatef(cam->pos.x,cam->pos.y,-1);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glEnable(GL_TEXTURE_2D);

		float mx = getGlXCoords(lastX);
		float my = getGlYCoords(lastY);
		int mt;
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
				//glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , data[mt].width(), data[mt].height(),  GL_RGBA, GL_UNSIGNED_BYTE, data[mt].bits() );
                                glBindTexture(GL_TEXTURE_2D, textures[mt]);
                                glBegin(GL_QUADS);
                                glTexCoord2f(0,1); glVertex2f(i,j+1);  // lower left
                                glTexCoord2f(0,0); glVertex2f(i,j); // lower right
                                glTexCoord2f(1,0); glVertex2f(i+1,j);// upper right
                                glTexCoord2f(1,1); glVertex2f(i+1,j+1); // upper left
                                glEnd();
                        }

                }

                const Sim::BotFactory::ObjVec &bots =  sim->getState().getBotFactory().getBotVector();

                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_BLEND);

                for (i = 0; i < bots.size(); i++) {
                        Sim::Bot *bot = bots[i];
                        if (bot != NULL) {
                                Sim::Vector vec = bot->getBody().mPos;
                                //glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , characters[mt].width(), characters[mt].height(),  GL_RGBA, GL_UNSIGNED_BYTE, characters[mt].bits() );
                                glBindTexture(GL_TEXTURE_2D, weaponstextures[0]);
                                glBegin(GL_QUADS);
                                glTexCoord2f(0,1); glVertex2f(vec.x+0.3,vec.y+1.8);  // lower left
                                glTexCoord2f(0,0); glVertex2f(vec.x+0.3,vec.y+1.3); // lower right
                                glTexCoord2f(1,0); glVertex2f(vec.x+1.3,vec.y+1.3);// upper right
                                glTexCoord2f(1,1); glVertex2f(vec.x+1.3,vec.y+1.8); // upper left
                                glEnd();
                                glBindTexture(GL_TEXTURE_2D, chartextures[0]);
                                glBegin(GL_QUADS);
                                glTexCoord2f(0,1); glVertex2f(vec.x,vec.y + 1.8);  // lower left
                                glTexCoord2f(0,0); glVertex2f(vec.x,vec.y); // lower right
                                glTexCoord2f(1,0); glVertex2f(vec.x+1,vec.y);// upper right
                                glTexCoord2f(1,1); glVertex2f(vec.x+1,vec.y+1.8); // upper left
                                glEnd();
                                glBindTexture(GL_TEXTURE_2D, 0);
                                /*glBlendFunc(GL_ONE, GL_ONE);
				//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

				glEnable(GL_TEXTURE_2D);*/

                        }
                }
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1, 1, -1, 1, 0.01, 1000);
		glTranslatef(0,0,-1);
		/*glBindTexture(GL_TEXTURE_2D, mousetextures[cMouse]);

		glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex2f(mx-mouseSize,my+mouseSize);  // lower left
		glTexCoord2f(0,0); glVertex2f(mx-mouseSize,my-mouseSize); // lower right
		glTexCoord2f(1,0); glVertex2f(mx+mouseSize,my-mouseSize);// upper right
		glTexCoord2f(1,1); glVertex2f(mx+mouseSize,my+mouseSize); // upper left
		glEnd();*/
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glFlush();
		glFinish();
                swapBuffers();
        }
        int getXBlock(double x) {
                return 0;
        }
	double getGlXCoords(int x) {
		return -1+(((double)x)/width())*2;
	}
	double getGlYCoords(int y) {
		return 1-(((double)y)/height())*2;
	}
	double getXPix(int x) {
		return getGlXCoords(x)-cam->pos.x;
        }
        double getYPix(int y) {
		return getGlYCoords(y)-cam->pos.y;
        }

};
#endif
