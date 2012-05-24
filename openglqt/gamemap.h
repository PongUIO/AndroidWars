#ifndef GAMEMAP_H
#define GAMEMAP_H
#include <QtGui>
#include "globj.h"
#include "Simulation.h"
#include "../util/client.h"

class Piece {
public:
	GLObj *r;
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
};

class GameMap {
public:
	GameMap(Camera *tmp) {
		map = NULL;
		offmap = -1;
		x = 0;
		y = 0;
		wld = NULL;
		cam = tmp;
	}
	void registerPiece(QString file, int x, int y, float z) {
		pieces.push_back(Piece(file, QVector3D(x,y,z)));
	}
	void registerPiece(GLObj *go, int x, int y, float z) {
		pieces.push_back(Piece(go, QVector3D(x,y,z)));
	}
	void updateDimensions() {
		if (!wld) {
			return;
		}
		x = wld->getWidth();
		y = wld->getHeight();
		int i, j;
		if (map) {
			delete map;
		}
		map = new int*[x];
		for (i = 0; i < x; i++) {
			map[i] = new int[y];
			for (j = 0; j < y; j++) {
				map[i][j] = 1;
			}
		}

	}
	void genMap() {

	}
	int getPiece(int x, int y) {
		if (0 <= x && x < this->x && 0 <= y && y < this->y) {
			return map[x][y];
		} else {
			return offmap;
		}
	}
	void setOffmap(int i) {
		offmap = i;
	}
	void setWorld(Sim::World *tmp) {
		wld = tmp;
		updateDimensions();
	}
	void draw() {
		int i,j;
		int mt;
		int fx = cam->xSimLim(-1);
		int tx = cam->xSimLim(1)+1;
		int fy = cam->ySimLim(1);
		int ty = cam->ySimLim(-1)+1;
//		glEnable(GL_TEXTURE_2D);
		for (i = fx; i < tx; i++) {
			for (j = fy; j < ty; j++) {
				mt = wld->getTile(i, j).getType();
				if (mt == 0) {
					mt = getPiece(i, j);
					if (mt != -1) {
						glColor4f(1.0f, 0.f, 0.f, 1.0f);
						pieces[mt-1].r->draw(i,j,-1);
					}
				} else {
					glColor4f(0.f, 1.0f, 0.f, 1.0f);
					//glBindTexture(GL_TEXTURE_2D, mousetextures[0]);
					//drawTexObj3d(i, j, i+1, j+1, 0);
					pieces[mt-1].r->draw(i, j, 0);
				}
			}
		}
	}
private:
	QVector<Piece> pieces;
	int **map;
	int x, y, offmap;
	Sim::World *wld;
	Camera *cam;
};

#endif // GAMEMAP_H
