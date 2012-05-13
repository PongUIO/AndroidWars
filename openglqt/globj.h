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
			float max[3] = {0, 0, 2};
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
					qDebug() << "next";
					for (i = 0; i < qsl.size()-1; i ++) {
						QStringList qsl2 = qsl.at(i+1).split("/");

						qDebug() << qsl2.at(0);
						indices.push_back(qsl2.at(0).toUInt()-1);
					}
				}
			}
			f.close();
			QVector3D minVec = QVector3D(min[0], min[1], min[2]);
			QVector3D maxVec = QVector3D(max[0], max[1], max[2]);
			qDebug() << minVec;
			qDebug() << maxVec;
			QVector3D diff = maxVec - minVec;
			QVector3D scaleCorrected = QVector3D(1./diff.x(), 1./diff.y(), 1./diff.z()) * scale;
			QVector3D offset = QVector3D(0.,0.,0.);
			for (i = 0; i < vertices.size(); i++) {
				vertices[i] = (vertices[i] - minVec)*scaleCorrected+offset;
				qDebug() << vertices[i];
				//qDebug() << vertices[i];
			}
			qDebug() << vertices[0];
			qDebug() << indices[0] << indices[1] << indices[2];
			bufVert = QGLBuffer(QGLBuffer::VertexBuffer);
			bufInd = QGLBuffer(QGLBuffer::IndexBuffer);
			qDebug() << bufVert.create();
			qDebug() << bufVert.bind();
			bufVert.allocate(&vertices[0], sizeof(QVector3D)*vertices.size());
			qDebug() << bufVert.size();
			bufVert.setUsagePattern(QGLBuffer::DynamicDraw);
			//bufVert.release();
			qDebug() << bufInd.create();
			qDebug() << bufInd.bind();
			bufInd.allocate(&indices[0], sizeof(GLuint)*indices.size());
			qDebug() << bufInd.size();
			bufInd.setUsagePattern(QGLBuffer::DynamicDraw);
			/*qDebug() << verticeBuff.create();
			verticeBuff.bind();
			verticeBuff.allocate( &vertices[0], vertices.size()*sizeof(QVector3D));
			qDebug() << indiceBuff.create();
			indiceBuff.bind();
			indiceBuff.allocate( &indices[0], indices.size()*sizeof(GLint));
			qDebug() << indiceBuff.isCreated();
			qDebug() << verticeBuff.isCreated();
			verticeBuff.setUsagePattern(QGLBuffer::StaticDraw);
			indiceBuff.setUsagePattern(QGLBuffer::StaticDraw);
			QVector3D testVec = QVector3D(0,0,0);
			qDebug() << vertices[1];
			qDebug() << verticeBuff.read(sizeof(testVec), &testVec, sizeof(QVector3D));
			qDebug() << testVec;*/

		//	indiceBuff.release();
		//	verticeBuff.release();
		}
		void init() {
		}
		void draw() {

			//glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);

			// Set viewpoint
			glLoadIdentity();
			glOrtho(-1, 1, -1, 1, -1, 1);

			glMatrixMode(GL_MODELVIEW);

			glLoadIdentity();

			bufVert.bind();

			bufInd.bind();
			glEnableClientState( GL_VERTEX_ARRAY );
			glVertexPointer(3, GL_FLOAT, 0, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 60);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glDisableClientState( GL_VERTEX_ARRAY );
			bufVert.release();
			bufInd.release();

			// Cleanup + show the user the triangles
	//		glFlush();
	//		glFinish();
//			swapBuffers();
			/*
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-1, 1, -1, 1, -1, 1);
			//glEnableClientState(GL_INDEX_ARRAY);
			glColor3f(1.,0.,1.);
			if (verticeBuff.bind()) {
				if (indiceBuff.bind())  {
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(3, GL_FLOAT, 0, 0);
					glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
					indiceBuff.release();
				}
				verticeBuff.release();
			}
			glDisableClientState(GL_VERTEX_ARRAY);*/
			//glDisableClientState(GL_INDEX_ARRAY);
			//glEndList();
		}
	void drawObj3d(double xf, double yf, double xt, double yt, double z) {
		glBegin(GL_QUADS);
		glVertex3f(xf,yt,z);
		glVertex3f(xf,yf,z);
		glVertex3f(xt,yf,z);
		glVertex3f(xt,yt,z);
		glEnd();
	}
};

#endif // GLOBJ_H
