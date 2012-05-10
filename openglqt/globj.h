#ifndef GLOBJ_H
#define GLOBJ_H
#include <QtOpenGL>

class GLObj {
	public:
		QVector<QVector3D> vertices;
		QVector<GLint> indices;
		QGLBuffer indiceBuff;
		QGLBuffer verticeBuff;
		GLObj(QString file, QVector3D scale) {
			//glNewList(list, GL_COMPILE);
			indiceBuff = QGLBuffer(QGLBuffer::IndexBuffer);
			verticeBuff = QGLBuffer(QGLBuffer::VertexBuffer);
			int i;
			QFile f(file);
			f.open(QFile::QIODevice::ReadOnly);
			float max[3] = {0, 0, 0};
			float min[3] = {0, 0, 0};
			while (!f.atEnd()) {
				QString str = f.readLine();
				QStringList qsl = str.split(QRegExp(" +"));
				if (qsl.at(0) == "v") {
					float arr[3];
					for (i = 0; i < 3; i ++) {
						arr[i] = qsl.at(i+1).toFloat();
						max[i] = ((max[i] < arr[i] || !max[i] ) ? arr[i] : max[i]);
						min[i] = ((min[i] > arr[i] || !min[i] ) ? arr[i] : min[i]);

					}
					vertices.push_back(QVector3D(arr[0], arr[1], arr[2]));
				} else if (qsl.at(0) == "f") {
					for (i = 0; i < 3; i ++) {
						indices.push_back(qsl.at(i+1).toInt());
					}
				}
			}
			QVector3D minVec = QVector3D(min[0], min[1], min[2]);
			QVector3D maxVec = QVector3D(max[0], max[1], max[2]);
			QVector3D diff = maxVec - minVec;
			QVector3D scaleCorrected = QVector3D(1./diff.x(), 1./diff.y(), 1./diff.z()) * scale;
			QVector3D offset = QVector3D(0.,0.,0.);
			for (i = 0; i < vertices.size(); i++) {
				vertices[i] = (vertices[i] - minVec)*scaleCorrected+offset;
				//qDebug() << vertices[i];
			}
			qDebug() << verticeBuff.create();
			verticeBuff.bind();
			verticeBuff.allocate( &vertices[0], vertices.size()*sizeof(QVector3D));
			qDebug() << indiceBuff.create();
			indiceBuff.allocate( &indices[0], indices.size()*sizeof(GLint));
			indiceBuff.bind();
			qDebug() << indiceBuff.isCreated();
			qDebug() << verticeBuff.isCreated();
			f.close();
			QVector3D testVec = QVector3D(0,0,0);
			qDebug() << vertices[1];
			qDebug() << verticeBuff.read(sizeof(testVec), &testVec, sizeof(QVector3D));
			qDebug() << testVec;
		}
		void init() {
		}
		void draw() {
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glColor3f(1.,1.,1.);
			if (verticeBuff.bind()) {
				if (indiceBuff.bind())  {
					glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
					indiceBuff.release();
				}
				verticeBuff.release();
			}
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glEndList();
		}
};

#endif // GLOBJ_H
