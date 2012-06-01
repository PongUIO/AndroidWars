#ifndef GLOBJ_H
#define GLOBJ_H
#include <QtOpenGL>

class CacheEntry {
public:
	int vert, tex, norm;
	CacheEntry() { vert = 0; tex = 0; norm = 0; }
	CacheEntry(int v, int t, int n) {
		vert = v;
		tex = t;
		norm = n;
	}
	bool operator == (const CacheEntry &other) const {
		return (other.norm == norm && other.vert == vert && other.tex == tex);
	}

};

class GLObj {
	public:
	QVector<QVector3D> vertices;
	QVector<GLuint> indices;
	size_t indNum;
        QGLBuffer bufInt, bufFloat;
        QVector3D minVec, maxVec;
        GLObj(QString file) {
                loadFile(file);
                initBuf();
        }
	GLObj(QString file, QVector3D scale) {
		loadFile(file);
		scaleAndCenter(scale);
		initBuf();
	}
	void draw(float x, float y, float z) {

		//glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);

		glTranslatef( x, y, z);
		glRotatef(90, 0,1,0);


		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		bufFloat.bind();
		glVertexPointer(3, GL_FLOAT, 3*sizeof(QVector3D), 0);
		glTexCoordPointer(3, GL_FLOAT,3*sizeof(QVector3D), (void *) sizeof(QVector3D));
		glNormalPointer(GL_FLOAT, 3*sizeof(QVector3D), (void *) (2*sizeof(QVector3D)));
		bufInt.bind();
		glIndexPointer(GL_UNSIGNED_INT, 2*sizeof(GLuint), 0);
		glDrawElements(GL_TRIANGLES, indNum, GL_UNSIGNED_INT, 0);
		bufFloat.release();
		bufInt.release();
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );

		glRotatef(-90, 0,1,0);
		glTranslatef( -x, -y, -z);
	}
	void loadFile(QString file) {
		QVector<GLuint> tmpInd;
		QVector<QVector3D> tmpVert, tmpTex, tmpNorm;
		QVector<CacheEntry> cache;
		int i, j, k;
		QFile f(file);
		f.open(QFile::QIODevice::ReadOnly);
		bool first = true;
		float max[3] = {0, 0, 0};
		float min[3] = {0, 0, 0};
		QRegExp reg(" +");
		while (!f.atEnd()) {
			QString str = f.readLine();
			QStringList qsl = str.split(reg);
			if (qsl.at(0) == "v") {
				float arr[3];
				for (i = 0; i < 3; i ++) {
					arr[i] = qsl.at(i+1).toFloat();

					max[i] = ((max[i] < arr[i] || !max[i] || first ) ? arr[i] : max[i]);
					min[i] = ((min[i] > arr[i] || first ) ? arr[i] : min[i]);

				}
				first = false;
				tmpVert.push_back(QVector3D(arr[0], arr[1], arr[2]));
			} else if (qsl.at(0) == "f") {
				for (i = 0; i < qsl.size()-1; i ++) {
					QStringList qsl2 = qsl.at(i+1).split("/");
					for (j =0; j < 3; j++) {
						tmpInd.push_back(qsl2.at(j).toUInt()-1);
					}
				}
			} else if (qsl.at(0) == "vt") {
				tmpTex.push_back(QVector3D(qsl.at(1).toFloat(), qsl.at(2).toFloat(), qsl.at(3).toFloat()));
			} else if (qsl.at(0) == "vn") {
				tmpNorm.push_back(QVector3D(qsl.at(1).toFloat(), qsl.at(2).toFloat(), qsl.at(3).toFloat()));
			}
		}
		f.close();
		bool found;
		k = 0;
		for (int i = 0; i < tmpInd.size(); i+=3) {
			found = false;
			CacheEntry tmpEntry = CacheEntry(tmpInd[i], tmpInd[i+1], tmpInd[i+2]);
			for (j = 0; j < cache.size(); j++) {
				if (cache[j] == tmpEntry) {
					found = true;
					break;
				}
			}
			if (!found) {
				vertices.push_back(tmpVert[tmpInd[i]]);
				vertices.push_back(tmpTex[tmpInd[i+1]]);
				vertices.push_back(tmpNorm[tmpInd[i+2]]);
				indices.push_back(k);
                                cache.push_back(tmpEntry);
				k++;
			} else {
				indices.push_back(j);
			}
		}
		minVec = QVector3D(min[0], min[1], min[2]);
		maxVec = QVector3D(max[0], max[1], max[2]);
	}
	void scaleAndCenter(QVector3D scale) {
		int i;
		QVector3D diff = maxVec - minVec;

		if ((scale - diff).length() < pow(10,-10)) {
			return;
		}
		QVector3D scaleCorrected = QVector3D(1./diff.x(), 1./diff.y(), 1./diff.z()) * scale;
		QVector3D offset = QVector3D(0.,0.,0.);
		for (i = 0; i < vertices.size(); i+=3) {
			vertices[i] = (vertices[i] - minVec)*scaleCorrected+offset;
		}
		minVec = QVector3D(0,0,0);
		maxVec = scale;
	}
	void initBuf() {
		bufFloat = QGLBuffer(QGLBuffer::VertexBuffer);
                bufInt = QGLBuffer(QGLBuffer::IndexBuffer);

                bufFloat.create();
                bufFloat.bind();
		bufFloat.allocate(&vertices[0], sizeof(QVector3D)*vertices.size());
                bufFloat.size();
                bufFloat.setUsagePattern(QGLBuffer::StaticDraw);

                bufInt.create();
                bufInt.bind();
		bufInt.allocate(&indices[0], sizeof(GLuint)*indices.size());
		indNum = indices.size();
                bufInt.size();
                bufInt.setUsagePattern(QGLBuffer::StaticDraw);
	}
};

#endif // GLOBJ_H
