#include<stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define Pl 3.1415926

int c;

struct ball{
  float x;
  float y;
  double r; //半径
  double vx; //x軸方向の速度
  double vy; //y軸方向の速度
  double ax; //x軸方向の加速度
  double ay; //y軸方向の加速度
};

struct ball b1[] ={
{-20.0,-30.0,5.0,0.1,0.09,0.0,0.0},
{-10.0,30.0,5.0,-0.15,0.08,0.0,0.0}
};

void display(void)
{ 
  glClear(GL_COLOR_BUFFER_BIT);

  //構造体を用いたボールの描画
   for(c = 0; c < 2 ; c++) {

    if(c == 0){
      glColor3d(0.3, 0.8, 0.7);
   }
    else{
      glColor3d(0.65, 0.25, 0.0);
    }

    glPushMatrix();
   glTranslatef(b1[c].x, b1[c].y, 0.0);
    glBegin(GL_POLYGON);  //塗りつぶし
  for( int i=0; i<=36; i++){
      glVertex2d(b1[c].r*cos( (i*Pl* 10.0)/180.0), b1[c].r*sin( (i*Pl* 10.0)/180.0));
  }
  glEnd();
  glPopMatrix();
  glFlush();
   } 
    glutSwapBuffers(); 
}

void simu(void)
{
  //ボールに速さを与える
 for(c = 0; c < 2 ; c++) {
  b1[c].x += b1[c].vx;
  b1[c].y += b1[c].vy;

//ボール同士の反射
 if((b1[1].x - b1[0].x)*(b1[1].x - b1[0].x) + (b1[1].y - b1[0].y)*(b1[1].y - b1[0].y) < (b1[1].r + b1[0].r)*(b1[1].r + b1[0].r)) {

  b1[c].vx *= -1;
  b1[c].vy *= -1;
 }

//ボールと壁の反射
  if(b1[c].x >= 45 || b1[c].x <= -45)
    b1[c].vx *= -1;
   if(b1[c].y >= 45 || b1[c].y <= -45)
    b1[c].vy *= -1;
  
    glutPostRedisplay();  
 }
}
 
void init(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void timer(int value){

  simu();
  glutPostRedisplay();
  glutTimerFunc(50, timer,0); //50m/s
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
