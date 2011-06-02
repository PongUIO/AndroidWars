#include<QtGui>
#include<qgl.h>
class MyGLDrawer : public QGLWidget {
	Q_OBJECT        // must include this if you use Qt signals/slots

	public:
		MyGLDrawer( QWidget *parent = 0)
			: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
			}

	protected:

		void initializeGL()
		{
			// Set up the rendering context, define display lists etc.:
			glClearColor( 0.0, 0.0, 0.0, 0.0 );
			glEnable(GL_DEPTH_TEST | GL_DOUBLE);
		}

		void resizeGL( int w, int h )
		{
			// setup viewport, projection etc.:
			glViewport( 0, 0, (GLint)w, (GLint)h );
			//glFrustum( ... );
		}

		void paintGL()
		{
			glClear(GL_COLOR_BUFFER_BIT);
			// draw the scene:
			//glRotatef( ... );
			//glMaterialfv( ... );
			glBegin( GL_TRIANGLES );
			glColor3f(0.5,0.5,0.5);
			glVertex2f(-1, -1);
			glVertex2f(1, 0);
			glVertex2f(-1,1);
			glEnd();
		}

};
