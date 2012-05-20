#ifndef GAMEMAP_H
#define GAMEMAP_H
#include <QtGui>
#include "globj.h"

class Piece {
public:
	Piece() {
		r = NULL;
	}
	Piece(QString file, QVector3D scale) {
		r = new GLObj(file, scale);
	}

	Piece(GLObj *go, QVector3D scale) {
		r = go;
		r->scaleAndCenter(scale);
	}
private:
	GLObj *r;
};

class GameMap {
public:
	GameMap() {
	}
	void registerPiece(QString file, int x, int y, float z) {
		pieces.push_back(Piece(file, QVector3D(x,y,z)));
	}
	void registerPiece(GLObj *go, int x, int y, float z) {
		pieces.push_back(Piece(go, QVector3D(x,y,z)));
	}
	void setDimensions(int x, int y) {
	}
	void genMap() {

	}
private:
	QVector<Piece> pieces;
};

#endif // GAMEMAP_H
