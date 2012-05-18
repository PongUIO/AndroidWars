#ifndef GLOBJ_H
#define GLOBJ_H
#include <QtOpenGL>

class GLObj {
	public:
		QVector<QVector3D> vertices;
		QVector<GLfloat> tex;
		QVector<GLfloat> norm;
		QVector<GLuint> indices;
		QGLBuffer bufInt;
		QGLBuffer bufFloat;
		GLObj(QString file, QVector3D scale) {
			//glNewList(list, GL_COMPILE);
			bufFloat = QGLBuffer(QGLBuffer::IndexBuffer);
			bufInt = QGLBuffer(QGLBuffer::VertexBuffer);
			bufFloat.setUsagePattern(QGLBuffer::StaticDraw);
			bufInt.setUsagePattern(QGLBuffer::StaticDraw);
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
					vertices.push_back(QVector3D(arr[0], arr[1], arr[2]));
				} else if (qsl.at(0) == "f") {
					for (i = 0; i < qsl.size()-1; i ++) {
						QStringList qsl2 = qsl.at(i+1).split("/");
						for (j =0; j < 1; j++) {
							indices.push_back(qsl2.at(j).toUInt()-1);
						}
					}
				} else if (qsl.at(0) == "vt") {
					for (i = 0; i < 3; i++) {
						tex.push_back(qsl.at(i+1).toFloat());
					}
				} else if (qsl.at(0) == "vt") {
					for (i = 0; i < 3; i++) {
						norm.push_back(qsl.at(i+1).toFloat());
					}
				}
			}
			f.close();
			QVector3D minVec = QVector3D(min[0], min[1], min[2]);
			QVector3D maxVec = QVector3D(max[0], max[1], max[2]);
			QVector3D diff = maxVec - minVec;
			QVector3D scaleCorrected = QVector3D(1./diff.x(), 1./diff.y(), 1./diff.z()) * scale;
			QVector3D offset = QVector3D(0.,0.,0.);
			for (i = 0; i < vertices.size(); i++) {
				vertices[i] = (vertices[i] - minVec)*scaleCorrected+offset;
			}
			bufFloat = QGLBuffer(QGLBuffer::VertexBuffer);
			bufInt = QGLBuffer(QGLBuffer::IndexBuffer);
			qDebug() << bufFloat.create();
			qDebug() << bufFloat.bind();
			bufFloat.allocate(sizeof(QVector3D)*vertices.size() + sizeof(GLfloat)*(tex.size() + norm.size()));
			bufFloat.write(0, &vertices[0] , sizeof(QVector3D)*vertices.size());
			bufFloat.write( sizeof(QVector3D)*vertices.size()*vertices.size(), &tex[0], sizeof(GLfloat)*tex.size());
			bufFloat.write( sizeof(QVector3D)*vertices.size()*vertices.size() + norm.size()*sizeof(GLfloat),
					&norm[0], sizeof(GLfloat)*norm.size());
			qDebug() << bufFloat.size();
			bufFloat.setUsagePattern(QGLBuffer::DynamicDraw);
			qDebug() << bufInt.create();
			qDebug() << bufInt.bind();
			bufInt.allocate(&indices[0], sizeof(GLuint)*indices.size());
			qDebug() << bufInt.size();
			bufInt.setUsagePattern(QGLBuffer::DynamicDraw);
		}
		void init() {
		}
		void draw(float x, float y) {

			//glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);

			glTranslatef( x, y, 0);
			glRotatef(90, 0,1,0);

			bufFloat.bind();

			bufInt.bind();
			glEnableClientState( GL_VERTEX_ARRAY );
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glIndexPointer(GL_UNSIGNED_INT, 2*sizeof(GLuint), 0);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glDisableClientState( GL_VERTEX_ARRAY );
			bufFloat.release();
			bufInt.release();

			glRotatef(-90, 0,1,0);
			glTranslatef( -x, -y, 0);
		}
};

#endif // GLOBJ_H
