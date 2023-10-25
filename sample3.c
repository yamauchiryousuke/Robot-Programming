#include<stdlib.h>
#include <GL/glut.h>

static GLfloat ang = 0.0;

void display(void)
{  
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 0.0);
  glPushMatrix();
  	glRotatef(ang, 0.0, 0.0, 1.0);
  	glRectf( -15.0, -15.0, 15.0, 15.0 );
  glPopMatrix();
  glutSwapBuffers();  
}

void simu(void)
{
  ang = ang + 1.0;
  if ( ang > 360.0 )
  	ang = ang - 360.0;
  glutPostRedisplay();
}

void init(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
  glViewport( 0, 0, w, h );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void mouse(int button, int state, int x, int y){
  switch (button) {
     case GLUT_LEFT_BUTTON:
     	if ( state == GLUT_DOWN)
     		glutIdleFunc(simu);
     	break;
     case GLUT_RIGHT_BUTTON:
		if ( state == GLUT_DOWN)
     		glutIdleFunc(NULL);
     	break; 
     default:
     	break; 
  }
}

void keyboard(unsigned char key, int x, int y)
{
  if ( key == '\x1b') exit(0);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (400, 400);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);
  init();
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutDisplayFunc(display);
    glutMainLoop();
  return 0;
}