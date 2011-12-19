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
	Q_OBJECT        // must include this if you use Qt signals/slots

public slots:
	void redraw() {
		paintGL();
	}

	void moveMouseCheck() {

		cam->addVel(lastX, lastY);
	}

public:
	int lastX, lastY, cMouse;
	double mouseSize;
	bool fullScreen;
	float selAlpha;
	bool dirAlpha;
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
	QImage bullet[1];
	GLuint bullettextures[1];
	QPixmap m;
	Sim::Simulation *sim;
	Sim::World *wld;
	Camera *cam;
	ClientStates *states;
	GameDrawer(Camera *cam, Sim::Simulation *simIn, ClientStates *states, QWidget *parent = 0)
		: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
		cMouse = 0;
		this->cam = cam;
		lastX = width()/2;
		lastY = height()/2;
		sim = simIn;
		wld = &sim->getState().getWorld();
		this->states = states;
		this->states->setSim(sim);
		setCursor( QCursor( Qt::BlankCursor ) );
		grabKeyboard();
		mouseSize = 0.07;
		selAlpha = 0.3;
		dirAlpha = false;
		fullScreen = false;
	}

protected:
	// overridden
	void keyPressEvent (QKeyEvent *event) {
		int k = event->key();
		if (k == Qt::Key_F11) {
			if (fullScreen) {
				showNormal();
			} else {
				showFullScreen();
			}
			fullScreen = !fullScreen;
		}
		modKey(k, true);
	}

	// overridden
	void keyReleaseEvent (QKeyEvent *event) {
		modKey(event->key(), false);
	}

	void modKey(int k, bool state) {
		if ( k == Qt::Key_Shift ) {
			states->setShift(state);
		} else if ( k == Qt::Key_Control ) {
			states->setCtrl(state);
			if (state) {

			}
		}
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
		states->registerClick(cam->xToSimX(event->x()), cam->yToSimY(event->y()), event->button());
	}



	// overridden
	void initializeGL() {
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

	void drawTexObj(double xf, double yf, double xt, double yt) {
		glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex2f(xf,yt);
		glTexCoord2f(0,0); glVertex2f(xf,yf);
		glTexCoord2f(1,0); glVertex2f(xt,yf);
		glTexCoord2f(1,1); glVertex2f(xt,yt);
		glEnd();
	}

	void drawObj(double xf, double yf, double xt, double yt) {
		glBegin(GL_QUADS);
		glVertex2f(xf,yt);
		glVertex2f(xf,yf);
		glVertex2f(xt,yf);
		glVertex2f(xt,yt);
		glEnd();
	}

	// overridden
	void paintGL() {
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
		int i, j;
		glClear(GL_COLOR_BUFFER_BIT);
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
				drawTexObj(i, j, i+1, j+1);
			}

		}

		const Sim::BotFactory::ObjVec &bots =  sim->getState().getBotFactory().getBotVector();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		for (i = 0; i < bots.size(); i++) {
			Sim::Bot *bot = bots[i];
			if (bot != NULL) {
				Sim::Vector pos = bot->getBody().mPos;
				Sim::Vector col = bot->getTypePtr()->getCollision()->getBboxHigh();
				//glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , characters[mt].width(), characters[mt].height(),  GL_RGBA, GL_UNSIGNED_BYTE, characters[mt].bits() );
				glBindTexture(GL_TEXTURE_2D, weaponstextures[0]);
				drawTexObj(pos.x+0.3, pos.y+1.3, pos.x+1.3, pos.y+1.3);

				glBindTexture(GL_TEXTURE_2D, chartextures[0]);
				drawTexObj(pos.x, pos.y, pos.x+col.x, pos.y+col.y);

				glBindTexture(GL_TEXTURE_2D, 0);
				/*glBlendFunc(GL_ONE, GL_ONE);
				//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

				glEnable(GL_TEXTURE_2D);*/

			}
		}
		glDisable(GL_TEXTURE_2D);
	glColor4f(0.2f, 1.0f, 0.2f, selAlpha);
		for (i = 0; i < bots.size(); i++) {
			if (states->isSelected(i)) {
				Sim::Bot *bot = bots[i];
				if (bot != NULL) {
					Sim::Vector pos = bot->getBody().mPos;
					Sim::Vector col = bot->getTypePtr()->getCollision()->getBboxHigh();
					drawObj(pos.x, pos.y, pos.x+col.x, pos.y+col.y);

				}
			}
		}
		glEnable(GL_TEXTURE_2D);

		const Sim::BulletFactory::ObjVec &bullets =  sim->getState().getBulletFactory().getObjVector();

		for (i = 0; i < bullets.size(); i++) {
			Sim::Bullet *bul = bullets[i];
			if (bul != NULL) {
				qDebug() << "working";
				Sim::Vector pos = bul->getBody().mPos;
				glBindTexture(GL_TEXTURE_2D, bullettextures[0]);
				drawTexObj(pos.x-0.005, pos.y-0.005, pos.x+0.005, pos.y+0.005);
			}
		}

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glFlush();
		glFinish();
		swapBuffers();
	}

};
#endif
