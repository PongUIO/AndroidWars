#ifndef RENDER_H
#define RENDER_H
#include<QtGui>
#include<QtOpenGL/qgl.h>
#include<iostream>
#include "Simulation.h"
#include "../util/camera.h"
#include "../util/client.h"
#include "../util/cursordefines.h"
#include "skeletalsystem.h"
#include "../Qt-based-common-libs/globj.h"
#include "gamemap.h"

class GameDrawer : public QGLWidget {
	Q_OBJECT

public slots:
	void redraw() {
		paintGL();
	}

	void tick() {
		if (mStates->menuOpen()) {
			return;
		}
		mCam->setLastPos(mLastX, mLastY);
		mCam->iter();
		if (mDirAlpha) {
			mSelAlpha += 0.01;
		} else {
			mSelAlpha -= 0.01;
		}
		if (mSelAlpha > 1) {
			mDirAlpha = false;
		} else if (mSelAlpha < 0.6) {
			mDirAlpha = true;
		}
	}

public:
	QWidget *mParent;
	int mLastX, mLastY, mCurrMouse;
	double mMouseSize;
	bool mFullScreen;
	float mSelAlpha;
	bool mDirAlpha;
	QGLShaderProgram *mTestShader;

	QVector<GLObj*> mRobots;
	QVector<GLObj*> mTerrain;
	QImage mData[3];
	QImage mWeapons[1];
	GLuint mWeaponstextures[1];
	QImage mBackground[1];
	GLuint mBGTextures[1];
	QImage mMouse[2];
	GLuint mMousetextures[2];
	QPixmap mMouseMaps[2];
	QImage mBullet[1];
	GLuint mBullettextures[1];
	QImage mCheckImage;
	GLuint mCheck;
	QImage mRedImage;
	GLuint mRed;

	Camera *mCam;
	ClientStates *mStates;
	QTimer *mGlTimer, *mCamTimer;
	GameMap *mGameMap;
	double mHitX, mHitY;
	QVector3D mScaleTest;
	GameDrawer(ClientStates *states, QWidget *parent = 0)
		: QGLWidget(QGLFormat(QGL::SampleBuffers), parent), mStates(states) {
		mScaleTest = QVector3D(1,1,1);
		this->mParent = parent;
		mCurrMouse = 0;
		this->mCam = new Camera(0, 0, parent->width(), parent->height());
		mLastX = width()/2;
		mLastY = height()/2;
		mMouseSize = 0.07;
		mSelAlpha = 0.8;
		mDirAlpha = false;
		mFullScreen = false;
		mGlTimer = new QTimer(parent);
		connect(mGlTimer, SIGNAL(timeout()), this, SLOT(redraw()));
		mCamTimer = new QTimer(parent);
		connect(mCamTimer, SIGNAL(timeout()), this, SLOT(tick()));
		mHitX = mHitY = 0;
		mGameMap = new GameMap(mCam);
	}

	void stopTimers() {
		mGlTimer->stop();
		mCamTimer->stop();
	}
	void startTimers() {
		mGlTimer->start(0);
		mCamTimer->start(40);
	}

	void modKeyState(int key, bool state) {
		switch (key) {
		case Qt::Key_W:
			mCam->setKeyMoveState(KEY_UP, state);
			break;
		case Qt::Key_S:
			mCam->setKeyMoveState(KEY_DOWN, state);
			break;
		case Qt::Key_A:
			mCam->setKeyMoveState(KEY_LEFT, state);
			break;
		case Qt::Key_D:
			mCam->setKeyMoveState(KEY_RIGHT, state);
			break;
		default:
			break;
		}
	}
protected:
	// overriden
	void mouseMoveEvent(QMouseEvent * event) {
		mLastX = event->pos().x();
		mLastY = event->pos().y();
	}

	// overridden
	void mousePressEvent(QMouseEvent * event) {
		if (mStates->menuOpen()) {
			return;
		}
		if (event->button() == Qt::MiddleButton) {
			mCam->stopZoom();
			return;
		}
		int w = width();
		int h = height();
		mHitX = mCam->xToSim(event->x());
		mHitY = mCam->yToSim(event->y());
		if (!(mStates->registerClick(mCam->xToSim(event->x()), mCam->yToSim(event->y()), event->button()) ||
				mStates->registerClick(mCam->xToSimBack(event->x()), mCam->yToSimBack(event->y()), event->button()))) {
			mCam->setDragMove(true);
		}
	}

	void mouseReleaseEvent(QMouseEvent *event) {
		mCam->setDragMove(false);
	}

	// overridden
	void initializeGL() {
		Sim::World *wld = &(mStates->getSim()->getSim().getState().getWorld());
		glClearColor( 0.1, 0.1, 0.1, 0.0 );
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DOUBLE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT );

		// Loading textures and obj-files.
		loadAndBind("../testmod/graphics/weapons/testweapon.png", &mWeapons[0], &mWeaponstextures[0], 32, 64);
		loadAndBind("../testmod/graphics/mouse/default.png", &mMouse[0], &mMousetextures[0],64,64);
		loadAndBind("../testmod/graphics/mouse/attack.png", &mMouse[1], &mMousetextures[1],64,64);
		loadAndBind("../testmod/graphics/weapons/bullet.png", &mBullet[0], &mBullettextures[0],16,16);
		loadAndBind("../testmod/graphics/debug/checker.png", &mCheckImage, &mCheck, 256, 256);
		loadAndBind("../testmod/graphics/debug/red.png", &mRedImage, &mRed, 100, 100);
		GLObj *tmp = new GLObj();
		tmp->loadFile("../testmod/obj/box.obj", QVector3D(1., 1., 1.));
		mGameMap->registerPiece(tmp);
		mGameMap->setWorld(wld);
		mGameMap->setOffmap(1);
		tmp = new GLObj();
		tmp->loadFile("../testmod/obj/Android01.obj", QVector3D(1., 1., 1.));
		mRobots.push_back(tmp);
		this->setAttribute(Qt::WA_NoSystemBackground);
		QPixmap m;
		m.convertFromImage(mMouse[MOUSE_NORMAL]);
		this->setCursor(QCursor(m, -1, -1));
		mTestShader = new QGLShaderProgram(context()->currentContext(), this);
		mTestShader->addShader(loadShader("../shaders/animation.vert", QGLShader::Vertex));
		mTestShader->addShader(loadShader("../shaders/animation.frag", QGLShader::Fragment));

		if (mTestShader) {
			mTestShader->link();
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
			ret->setProperty("tex", mRed);
			return ret;
		} else {
			qDebug() << "Shader compile error" << ret->log();
			return NULL;
		}
	}

	// overridden
	void resizeGL( int w, int h) {
		glViewport( 0, 0, (GLint)w, (GLint)h);
		mCam->calcRatio(w, h);
	}

	void resizeEvent(QResizeEvent *event) {
		resize(event->size().width(), event->size().height());
		resizeGL(event->size().width()
			 , event->size().height());
	}

	void wheelEvent(QWheelEvent *event) {
		mCam->modZoom(event->delta());
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
		exts::ExtSim *sim = mStates->getSim();

		mTestShader->bind();
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1, 1, -1*mCam->mRatio, 1*mCam->mRatio, 1, 5.1+mCam->mZoom);
		glTranslatef(mCam->mPos.x,mCam->mPos.y,-1-mCam->mZoom);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		std::list<Sim::Bot*> bots = sim->getSim().getState().getBotFactory().getBotList();
		std::list<Sim::Bot*>::iterator bot;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mCheck);
		mTestShader->bind();
		mTestShader->setAttributeValue("testtween", 0.5);
		mGameMap->draw();
		for (bot = bots.begin(); bot != bots.end(); bot++) {
			Sim::Vector pos = (*bot)->getBody().mPos;
			if (mStates->isSelected((*bot)->getId())) {

				glColor4f(0.2f, 1.0f, 0.2f, mSelAlpha);
			} else {
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			}
			mRobots[(*bot)->getTypeId()]->draw(pos.x, pos.y, 0);

		}
		mTestShader->release();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glFlush();
		glFinish();
		swapBuffers();
	}

};
#endif
