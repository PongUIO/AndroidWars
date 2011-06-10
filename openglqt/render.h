#include<QtGui>
#include<qgl.h>
#include<iostream>

class MyGLDrawer : public QGLWidget {
	Q_OBJECT        // must include this if you use Qt signals/slots

	public:
		int w, h;
		double cx, cy;
		MyGLDrawer( QWidget *parent = 0)
			: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
				cx = 1; // Current x
				cy = 1; // Current y
			}

	protected:
		// overridden
		void keyPressEvent (QKeyEvent *event) {
			std::cout << event->key() << std::endl;
		}

		// overridden
		void mousePressEvent(QMouseEvent * event) {
			w = width();
			h = height();
			cx = -1+(((double)event->x())/width())*2;
			cy = 1-(((double)event->y())/height())*2;
			std::cout << cx << " " << cy << std::endl << width() << " " << height() << std::endl;
			paintGL();
		}

		
			
		// overridden
		void initializeGL()
		{
			// Set up the rendering context, define display lists etc.:
			glClearColor( 0.0, 0.0, 0.0, 0.0 );
			glEnable(GL_DEPTH_TEST | GL_DOUBLE);
		}

		// overridden
		void resizeGL( int w, int h )
		{
			// setup viewport, projection etc.:
			glViewport( 0, 0, (GLint)w, (GLint)h );
			//glFrustum( ... );
		}

		// overridden
		void paintGL()
		{
			glClear(GL_COLOR_BUFFER_BIT);
			// draw the scene:
			//glRotatef( ... );
			//glMaterialfv( ... );
			glBegin( GL_TRIANGLES );
			glColor3f(0.5,0.5,0.5);
			glVertex2f(-1, -1);
			glVertex2f(1, 1);
			glVertex2f(cx,cy);
			glEnd();
			swapBuffers();
		}

};
