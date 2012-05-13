#ifndef GLOBJ_H
#define GLOBJ_H
#include <QtOpenGL>

class GLObj {
	public:
		QVector<QVector3D> vertices;
		QVector<GLuint> indices;
		QGLBuffer indiceBuff;
		QGLBuffer verticeBuff;
		QGLBuffer bufVert, bufInd;
		GLObj(QString file, QVector3D scale) {
			//glNewList(list, GL_COMPILE);
			indiceBuff = QGLBuffer(QGLBuffer::IndexBuffer);
			verticeBuff = QGLBuffer(QGLBuffer::VertexBuffer);
			indiceBuff.setUsagePattern(QGLBuffer::StaticDraw);
			verticeBuff.setUsagePattern(QGLBuffer::StaticDraw);
			int i, j;
			QFile f(file);
			f.open(QFile::QIODevice::ReadOnly);
			float max[3] = {-9999, -9999, -9999};
			float min[3] = {9999, 9999, 9999};
			while (!f.atEnd()) {
				QString str = f.readLine();
				QStringList qsl = str.split(QRegExp(" +"));
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
						indices.push_back(qsl2.at(0).toUInt()-1);
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
			bufVert = QGLBuffer(QGLBuffer::VertexBuffer);
			bufInd = QGLBuffer(QGLBuffer::IndexBuffer);
			qDebug() << bufVert.create();
			qDebug() << bufVert.bind();
			bufVert.allocate(&vertices[0], sizeof(QVector3D)*vertices.size());
			qDebug() << bufVert.size();
			bufVert.setUsagePattern(QGLBuffer::DynamicDraw);
			qDebug() << bufInd.create();
			qDebug() << bufInd.bind();
			bufInd.allocate(&indices[0], sizeof(GLuint)*indices.size());
			qDebug() << bufInd.size();
			bufInd.setUsagePattern(QGLBuffer::DynamicDraw);
		}
		void init() {
		}
		void draw(float x, float y) {

			//glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);

			glTranslatef( x, y, 0);
			glRotatef(90, 0,1,0);

			glColor3f(1., 1., 1.);
			bufVert.bind();

			bufInd.bind();
			glEnableClientState( GL_VERTEX_ARRAY );
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glDisableClientState( GL_VERTEX_ARRAY );
			bufVert.release();
			bufInd.release();

			glRotatef(-90, 0,1,0);
			glTranslatef( -x, -y, 0);
		}
};

#endif // GLOBJ_H
