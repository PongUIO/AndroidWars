#ifndef RENDER_H
#define RENDER_H
#include<QtGui>
#include<QtOpenGL/qgl.h>
#include<iostream>
#include "Simulation.h"
#include "../util/camera.h"
#include "../util/client.h"
#include "../util/cursordefines.h"
#include "globj.h"
#include "gamemap.h"

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
			selAlpha += 0.01;
		} else {
			selAlpha -= 0.01;
		}
		if (selAlpha > 1) {
			dirAlpha = false;
		} else if (selAlpha < 0.6) {
			dirAlpha = true;
		}
	}

public:
	QWidget *parent;
	int lastX, lastY, cMouse;
	double mouseSize;
	bool fullScreen;
	float selAlpha;
	bool dirAlpha;
	QGLShaderProgram *testShader;

	QVector<GLObj*> robots;
	QVector<GLObj*> terrain;
	QImage data[3];
	QImage weapons[1];
	GLuint weaponstextures[1];
	QImage bg[1];
	GLuint bgtextures[1];
	QImage mouse[2];
	GLuint mousetextures[2];
	QPixmap mouseMaps[2];
	QImage bullet[1];
	GLuint bullettextures[1];
	QImage checkImage;
	GLuint check;
	QImage redImage;
	GLuint red;

	Camera *cam;
	ClientStates *states;
	QTimer *glTimer, *camTimer;
	GameMap *gm;
	double hitX, hitY;
	QVector3D scaleTest;
	GameDrawer(ClientStates *states, QWidget *parent = 0)
		: QGLWidget(QGLFormat(QGL::SampleBuffers), parent), states(states) {
		scaleTest = QVector3D(1,1,1);
		this->parent = parent;
		cMouse = 0;
		this->cam = new Camera(0, 0, parent->width(), parent->height());
		lastX = width()/2;
		lastY = height()/2;
		mouseSize = 0.07;
		selAlpha = 0.8;
		dirAlpha = false;
		fullScreen = false;
		glTimer = new QTimer(parent);
		connect(glTimer, SIGNAL(timeout()), this, SLOT(redraw()));
		camTimer = new QTimer(parent);
		connect(camTimer, SIGNAL(timeout()), this, SLOT(tick()));
		hitX = hitY = 0;
		gm = new GameMap(cam);
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
		if (!states->registerClick(cam->xToSimX(event->x()), cam->yToSimY(event->y()), event->button() == Qt::LeftButton)) {
			states->registerClick(cam->xToSimXBack(event->x()), cam->yToSimYBack(event->y()), event->button() == Qt::LeftButton);

		}
	}


	// overridden
	void initializeGL() {
		Sim::World *wld = &(states->getSim()->getState().getWorld());
		glClearColor( 0.1, 0.1, 0.1, 0.0 );
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DOUBLE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT );

		// Loading textures and obj-files.
		loadAndBind("../testmod/graphics/weapons/testweapon.png", &weapons[0], &weaponstextures[0], 32, 64);
		loadAndBind("../testmod/graphics/mouse/default.png", &mouse[0], &mousetextures[0],64,64);
		loadAndBind("../testmod/graphics/mouse/attack.png", &mouse[1], &mousetextures[1],64,64);
		loadAndBind("../testmod/graphics/weapons/bullet.png", &bullet[0], &bullettextures[0],16,16);
		loadAndBind("../testmod/graphics/debug/checker.png", &checkImage, &check, 256, 256);
		loadAndBind("../testmod/graphics/debug/red.png", &redImage, &red, 100, 100);
		gm->registerPiece(new GLObj("../testmod/obj/box.obj", QVector3D(1., 1., 1.)));
		gm->setWorld(wld);
		gm->setOffmap(1);
		robots.push_back(new GLObj("../testmod/obj/Android01.obj", QVector3D(1., 1., 1.)));
		this->setAttribute(Qt::WA_NoSystemBackground);
		QPixmap m;
		m.convertFromImage(mouse[MOUSE_NORMAL]);
		this->setCursor(QCursor(m, -1, -1));
		testShader = new QGLShaderProgram(context()->currentContext(), this);
		testShader->addShader(loadShader("../shaders/animation.vert", QGLShader::Vertex));
		testShader->addShader(loadShader("../shaders/animation.frag", QGLShader::Fragment));

		if (testShader) {
			testShader->link();
		}
		glDisable(GL_TEXTURE_2D);

	}
	void loadAndBind(QString path, QImage *img, GLuint *bind, GLuint xsize = -1, GLuint ysize = -1, bool vertFlip = false, bool horFlip = false) {
		if (xsize != -1) {
			img->load(path);
			*img = (*img).scaled(xsize, ysize).mirrored(horFlip, vertFlip);
		} else {
			img->load(path);
		}
		*bind = bindTexture(*img);
	}

	QGLShader* loadShader(QString path, QGLShader::ShaderType type) {
		QGLShader *ret = new QGLShader(type, this);
		if (ret->compileSourceFile(path)) {
			qDebug() << ret->sourceCode();
			ret->setProperty("tex", red);
			return ret;
		} else {
			qDebug() << "Shader compile error" << ret->log();
			return NULL;
		}
	}

	// overridden
	void resizeGL( int w, int h) {
		glViewport( 0, 0, (GLint)w, (GLint)h);
		cam->calcRatio(w, h);
	}

	void resizeEvent(QResizeEvent *event) {
		resize(event->size().width(), event->size().height());
		resizeGL(event->size().width()
			 , event->size().height());
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
		testShader->bind();
		Sim::Simulation *sim = states->getSim();
		Sim::World *wld = &(states->getSim()->getState().getWorld());
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1, 1, -1*cam->ratio, 1*cam->ratio, 1, 5.1+cam->zoom);
		glTranslatef(cam->pos.x,cam->pos.y,-1-cam->zoom);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		std::list<Sim::Bot*> bots = sim->getState().getBotFactory().getBotList();
		std::list<Sim::Bot*>::iterator bot;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, check);
		testShader->bind();
		gm->draw();
		for (bot = bots.begin(); bot != bots.end(); bot++) {
			Sim::Vector pos = (*bot)->getBody().mPos;
			Sim::Vector col = (*bot)->getTypePtr()->getCollision()->getBboxHigh();
			if (states->isSelected((*bot)->getId())) {

				glColor4f(0.2f, 1.0f, 0.2f, selAlpha);
			} else {
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			}
			robots[(*bot)->getTypeId()]->draw(pos.x, pos.y, 0);

		}
		testShader->release();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glFlush();
		glFinish();
		swapBuffers();
	}

};
#endif
