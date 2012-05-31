#ifndef GLOBJ_H
#define GLOBJ_H
#include <QtOpenGL>

class GLObj {
	public:
	QVector<QVector3D> vertices;
	QVector<GLuint> indices;
	QGLBuffer bufInt;
	QGLBuffer bufFloat;
	QVector3D minVec, maxVec;
	GLObj(QString file, QVector3D scale) {
		//glNewList(list, GL_COMPILE);
		bufFloat = QGLBuffer(QGLBuffer::IndexBuffer);
		bufInt = QGLBuffer(QGLBuffer::VertexBuffer);

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
		//glNormalPointer(GL_FLOAT, 3*sizeof(QVector3D), (void *) (2*sizeof(QVector3D)));
		bufInt.bind();
		glIndexPointer(GL_UNSIGNED_INT, 2*sizeof(GLuint), 0);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		bufFloat.release();
		bufInt.release();
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );

		glRotatef(-90, 0,1,0);
		glTranslatef( -x, -y, -z);
	}
	void loadFile(QString file) {
		QVector<GLuint> temp;
		QVector<QVector3D> tmpVert, tmpTex, tmpNorm;
		int i, j;
		QFile f(file);
		f.open(QFile::QIODevice::ReadOnly);
		float max[3] = {-9999, -9999, -9999};
		float min[3] = {9999, 9999, 9999};
		QRegExp reg(" +");
		while (!f.atEnd()) {
			QString str = f.readLine();
			QStringList qsl = str.split(reg);
			if (qsl.at(0) == "v") {
				float arr[3];
				for (i = 0; i < 3; i ++) {
					arr[i] = qsl.at(i+1).toFloat();

					max[i] = ((max[i] < arr[i] || !max[i] ) ? arr[i] : max[i]);
					min[i] = ((min[i] > arr[i] ) ? arr[i] : min[i]);

				}
				tmpVert.push_back(QVector3D(arr[0], arr[1], arr[2]));
			} else if (qsl.at(0) == "f") {
				for (i = 0; i < qsl.size()-1; i ++) {
					QStringList qsl2 = qsl.at(i+1).split("/");
					for (j =0; j < 3; j++) {
						temp.push_back(qsl2.at(j).toUInt()-1);
					}
				}
			} else if (qsl.at(0) == "vt") {
				tmpTex.push_back(QVector3D(qsl.at(1).toFloat(), qsl.at(2).toFloat(), qsl.at(3).toFloat()));
			} else if (qsl.at(0) == "vn") {
				tmpNorm.push_back(QVector3D(qsl.at(1).toFloat(), qsl.at(2).toFloat(), qsl.at(3).toFloat()));
			}
		}
		f.close();
		for (int i = 0; i < temp.size(); i+=3) {
			vertices.push_back(tmpVert[temp[i]]);
			vertices.push_back(tmpTex[temp[i+1]]);
			vertices.push_back(tmpNorm[temp[i+2]]);
			indices.push_back(i/3);
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
		qDebug() << bufFloat.create();
		qDebug() << bufFloat.bind();
		//bufFloat.allocate(&vertices[0], sizeof(QVector3D)*vertices.size());
		bufFloat.allocate(sizeof(QVector3D)*vertices.size());
		bufFloat.write(0, &vertices[0] , sizeof(QVector3D)*vertices.size());
		qDebug() << bufFloat.size();
		bufFloat.setUsagePattern(QGLBuffer::DynamicDraw);
		qDebug() << bufInt.create();
		qDebug() << bufInt.bind();
		bufInt.allocate(&indices[0], sizeof(GLuint)*indices.size());
		qDebug() << bufInt.size();
		bufInt.setUsagePattern(QGLBuffer::DynamicDraw);
	}
};

#endif // GLOBJ_H
