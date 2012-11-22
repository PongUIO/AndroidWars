#ifndef GAMEMAP_H
#define GAMEMAP_H
#include <QtGui>
#include "../Qt-based-common-libs/globj.h"
#include "Simulation.h"
#include "../util/client.h"

class Piece {
public:
	GLObj *mObj;
	Piece() {
		mObj = NULL;
	}
	Piece(QString file, QVector3D scale) {
		mObj = new GLObj();
		mObj->loadFile(file, scale);
	}

	Piece(GLObj *go, QVector3D scale) {
		mObj = go;
		mObj->scaleAndCenter(scale);
	}
	Piece(GLObj *go) {
		mObj = go;
	}
};

class GameMap {
public:
	GameMap(Camera *tmp) {
		mMap = NULL;
		mOffmap = -1;
		mX = 0;
		mY = 0;
		mWld = NULL;
		mCam = tmp;
	}
	void registerPiece(QString file, int x, int y, float z) {
		mPieces.push_back(Piece(file, QVector3D(x,y,z)));
	}
	void registerPiece(GLObj *go, int x, int y, float z) {
		mPieces.push_back(Piece(go, QVector3D(x,y,z)));
	}
	void registerPiece(GLObj *go) {
		mPieces.push_back(Piece(go));
	}
	void updateDimensions() {
		if (!mWld) {
			return;
		}
		mX = mWld->getWidth();
		mY = mWld->getHeight();
		int i, j;
		if (mMap) {
			delete mMap;
		}
		mMap = new int*[mX];
		for (i = 0; i < mX; i++) {
			mMap[i] = new int[mY];
			for (j = 0; j < mY; j++) {
				mMap[i][j] = 1;
			}
		}

	}
	void genMap() {

	}
	bool inMap(int x, int y) {
		return ((0 <= x && x < this->mX && 0 <= y && y < this->mY) ? 1 : 0);
	}
	int getPiece(int x, int y) {
		return mMap[x][y];
	}
	void setOffmap(int i) {
		mOffmap = i;
	}
	void setWorld(Sim::World *tmp) {
		mWld = tmp;
		updateDimensions();
	}
	void draw(QGLShaderProgram *shader, QMatrix4x4 *m) {
		int i,j;
		int mt;
		int fx = mCam->xSimLim(-1);
		int tx = mCam->xSimLim(1)+1;
		int fy = mCam->ySimLim(1);
		int ty = mCam->ySimLim(-1)+1;
		shader->setUniformValue("edgeColor", QVector4D(0.,0.,1.0,1.0));
		for (i = fx; i < tx; i++) {
			for (j = fy; j < ty; j++) {
				if (inMap(i, j)) {
					mt = mWld->getTile(i, j).getType();
				} else {
					mt = mOffmap;
				}
				if (mt == 0) {
					mt = getPiece(i, j);
					if (mt != -1) {
						shader->setUniformValue("color", QVector4D(1.0, 0., 0., 1.0));
						mPieces[mt-1].mObj->draw(i, j, -1, shader, "projection_matrix", m);
					}
				} else {
					if (mt > 0) {
						shader->setUniformValue("color", QVector4D(0., 0., 0., 1.0));
						mPieces[mt-1].mObj->draw(i, j, 0, shader, "projection_matrix", m);
					}
				}
			}
		}
	}
private:
	QVector<Piece> mPieces;
	int **mMap;
	int mX, mY, mOffmap;
	Sim::World *mWld;
	Camera *mCam;
};

#endif // GAMEMAP_H
