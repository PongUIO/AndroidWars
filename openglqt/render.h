#ifndef RENDER_H
#define RENDER_H
#include<QtGui>
#include<qgl.h>
#include<iostream>
#include "Simulation.h"
#include "../util/camera.h"
#include "../util/client.h"
#include "../util/cursordefines.h"

class GameDrawer : public QGLWidget {
        Q_OBJECT

public slots:
	void redraw() {
		paintGL();
	}

        void tick() {
                if (states->menuOpen()) {
                        return;
                }
                cam->setLastPos(lastX, lastY);
                cam->iter();
                if (dirAlpha) {
                        selAlpha += 0.003;
                } else {
                        selAlpha -= 0.003;
                }
                if (selAlpha > 0.5) {
                        dirAlpha = false;
                } else if (selAlpha < 0.2) {
                        dirAlpha = true;
                }
        }

public:
	int lastX, lastY, cMouse;
	double mouseSize;
	bool fullScreen;
	float selAlpha;
	bool dirAlpha;
	QWidget *parent;
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
	QPixmap mouseMaps[2];
	QImage bullet[1];
	GLuint bullettextures[1];
	Camera *cam;
        ClientStates *states;
        QTimer *glTimer, *camTimer;
	double hitX, hitY;
        GameDrawer(ClientStates *states, QWidget *parent = 0)
                : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
                this->parent = parent;
		cMouse = 0;
		this->cam = new Camera(0, 0, parent->width(), parent->height());
		lastX = width()/2;
		lastY = height()/2;
		this->states = states;
		mouseSize = 0.07;
		selAlpha = 0.3;
		dirAlpha = false;
		fullScreen = false;
                glTimer = new QTimer(parent);
                connect(glTimer, SIGNAL(timeout()), this, SLOT(redraw()));
                camTimer = new QTimer(parent);
                connect(camTimer, SIGNAL(timeout()), this, SLOT(tick()));
		hitX = hitY = 0;
        }

        void stopTimers() {
                glTimer->stop();
                camTimer->stop();
        }
        void startTimers() {
                glTimer->start(0);
                camTimer->start(40);
        }
protected:
	// overriden
        void mouseMoveEvent(QMouseEvent * event) {
                lastX = event->pos().x();
		lastY = event->pos().y();
	}

	// overridden
        void mousePressEvent(QMouseEvent * event) {
                if (states->menuOpen()) {
                        return;
                }
                int w = width();
                int h = height();
		hitX = cam->xToSimX(event->x());
		hitY = cam->yToSimY(event->y());
		states->registerClick(cam->xToSimX(event->x()), cam->yToSimY(event->y()), event->button());
	}



	// overridden
	void initializeGL() {
		// Set up the rendering context, define display lists etc.:
		glClearColor( 0.0, 0.0, 0.0, 0.0 );
		//glEnable(GL_DEPTH_TEST);
                glEnable(GL_DOUBLE) ;

		//Loading textures.
		loadAndBind(":/graphics/tiles/empty.png", &data[0], &textures[0]);
		loadAndBind(":/graphics/tiles/metal.png", &data[1], &textures[1]);
		loadAndBind(":/graphics/tiles/metal2surf.png", &data[2], &textures[2]);
		loadAndBind(":/graphics/characters/character1.png", &characters[0], &chartextures[0], 128, 320, false, true);
		loadAndBind(":/graphics/weapons/testweapon.png", &weapons[0], &weaponstextures[0], 32, 64);
		loadAndBind(":/graphics/mouse/default.png", &mouse[0], &mousetextures[0],64,64);
		loadAndBind(":/graphics/mouse/attack.png", &mouse[1], &mousetextures[1],64,64);
		loadAndBind(":/graphics/weapons/bullet.png", &bullet[0], &bullettextures[0],16,16);
		this->setAttribute(Qt::WA_NoSystemBackground);
		QPixmap m;
		m.convertFromImage(mouse[MOUSE_NORMAL]);
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
        void resizeGL( int w, int h) {
		glViewport( 0, 0, (GLint)w, (GLint)h);
		cam->calcRatio(w, h);
	}

        void resizeEvent(QResizeEvent *event) {
                resize(event->size().width(), event->size().height());
                resizeGL(event->size().width(), event->size().height());
	}

	void wheelEvent(QWheelEvent *event) {
		cam->modZoom(event->delta());
	}

	void drawTexObj2d(double xf, double yf, double xt, double yt) {
		glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex2f(xf,yt);
		glTexCoord2f(0,0); glVertex2f(xf,yf);
		glTexCoord2f(1,0); glVertex2f(xt,yf);
		glTexCoord2f(1,1); glVertex2f(xt,yt);
		glEnd();
	}
	void drawTexObj3d(double xf, double yf, double xt, double yt, double z) {
		glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex3f(xf,yt,z);
		glTexCoord2f(0,0); glVertex3f(xf,yf,z);
		glTexCoord2f(1,0); glVertex3f(xt,yf,z);
		glTexCoord2f(1,1); glVertex3f(xt,yt,z);
		glEnd();
	}

	void drawObj2d(double xf, double yf, double xt, double yt) {
		glBegin(GL_QUADS);
		glVertex2f(xf,yt);
		glVertex2f(xf,yf);
		glVertex2f(xt,yf);
		glVertex2f(xt,yt);
		glEnd();
	}
	void drawObj3d(double xf, double yf, double xt, double yt, double z) {
		glBegin(GL_QUADS);
		glVertex3f(xf,yt,z);
		glVertex3f(xf,yf,z);
		glVertex3f(xt,yf,z);
		glVertex3f(xt,yt,z);
		glEnd();
	}


	// overridden
        void paintGL() {
                Sim::Simulation *sim = states->getSim();
		Sim::World *wld = &(states->getSim()->getState().getWorld());
		int i, j;
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glFrustum(-cam->zoom, cam->zoom, -cam->zoom*cam->ratio, cam->zoom*cam->ratio, 0.5, 2);
		glFrustum(-1, 1, -1*cam->ratio, 1*cam->ratio, 0.5, 2.1+cam->zoom);
		glTranslatef(cam->pos.x,cam->pos.y,-1-cam->zoom);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor4f(1.0f, 1.0f, 0.f, 1.0f);
		glBegin(GL_TRIANGLES);
		glVertex3f(hitX,hitY,0);
		glVertex3f(hitX,hitY+0.2,0);
		glVertex3f(hitX+0.2,hitY+0.2,0);
		glEnd();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		std::list<Sim::Bot*> bots = sim->getState().getBotFactory().getBotList();
		std::list<Sim::Bot*>::iterator bot;

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		for (bot = bots.begin(); bot != bots.end(); bot++) {
			Sim::Vector pos = (*bot)->getBody().mPos;
			Sim::Vector col = (*bot)->getTypePtr()->getCollision()->getBboxHigh();
			//glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , characters[mt].width(), characters[mt].height(),  GL_RGBA, GL_UNSIGNED_BYTE, characters[mt].bits() );
			//glBindTexture(GL_TEXTURE_2D, weaponstextures[0]);
			//drawTexObj3d(pos.x+0.3, pos.y+1.3, pos.x+1.3, pos.y+1.3);

			glBindTexture(GL_TEXTURE_2D, chartextures[0]);
			drawTexObj3d(pos.x, pos.y, pos.x+col.x, pos.y+col.y, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
			/*glBlendFunc(GL_ONE, GL_ONE);
			//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_TEXTURE_2D);*/

		}
		glDisable(GL_TEXTURE_2D);

		glDisable(GL_TEXTURE_2D);
		glColor4f(0.2f, 1.0f, 0.2f, selAlpha);
		for (bot = bots.begin(); bot != bots.end(); bot++) {
			if (states->isSelected((*bot)->getId())) {
				Sim::Vector pos = (*bot)->getBody().mPos;
				Sim::Vector col = (*bot)->getTypePtr()->getCollision()->getBboxHigh();
				drawObj3d(pos.x, pos.y, pos.x+col.x, pos.y+col.y,0);
			}
		}
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-cam->zoom, cam->zoom, -cam->zoom*cam->ratio, cam->zoom*cam->ratio, 0.01, 1000);
		glTranslatef(cam->pos.x,cam->pos.y,-1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		float mx = cam->xPixToDouble(lastX);
		float my = cam->yPixToDouble(lastY);
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
				drawTexObj2d(i, j, i+1, j+1);
			}

		}


		glEnable(GL_TEXTURE_2D);

		glLoadIdentity();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glFlush();
		glFinish();
		swapBuffers();
	}

};
#endif
