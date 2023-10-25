#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926
#define G 9.80655
#define U 0.004
#define e 0.8  //wall反発係数//
#define el 0.8  //leg反発係数//

int i;
int f;  //向き判定　右：0左：1//

double x=-30,y=30;  //ball原点//
double r=5;  //ball半径//
double bx,by;  //ball側//

double lx=-50,ly=-50,tmpx,tmpy;  //leg原点//
double tx,ty;  //toe原点//
double lr=4;  //toe半径//
double ltx,lty;  //toe側//

double mb=1,ml=60;  //質量(kg)//

double mx=0,my=0;  //変化量//
double disx,disy,disbl;  //2つの原点との差//
double dt=0.03;  //時間変化量(s)//
double w;  //角速度//

double Vx=60,Vy=20;  //ball初速度(m/s)//
double Vl,Vlx,Vly; //leg速度(m/s)//

double friction;  //摩擦力//
double xw=95,yw=95;  //壁//

double sens=3;

static GLfloat ang = 0.0, angv=1.0;

void ball(void){
    glBegin(GL_POLYGON);
      glColor4f(1.0,1.0,0.0,0.0);
      /*ballの側*/
      for(i=0;i<=36;i++){
          bx=r*cos(10*i*PI/180.0);
          by=r*sin(10*i*PI/180.0);
      glVertex2d(bx,by);
      }
      glEnd();
}

void leg(void){
    glBegin(GL_QUADS);
      glColor4f(1.0,0.0,0.0,0.0);
      glVertex2d(0,0);
      glVertex2d(-2*lr,0);
      glVertex2d(-2*lr,-6*lr);
      glVertex2d(0,-6*lr);
      glVertex2d(lr,-6*lr);
      glVertex2d(-2*lr,-6*lr);
      glVertex2d(-2*lr,-8*lr);
      glVertex2d(lr,-8*lr);
      glEnd();
}

void toe(void){
    glBegin(GL_POLYGON);
      glColor4f(1.0,0.0,0.0,0.0);
      /*toeの側*/
      for(i=0;i<=36;i++){
          ltx=lr*cos(10*i*PI/180.0);
          lty=lr*sin(10*i*PI/180.0);
      glVertex2d(ltx,lty);
      }
      glEnd();
}

void display(void)
{

 /*ball*/
  glPushMatrix();
      glClear(GL_COLOR_BUFFER_BIT);
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1,0xACF3);
      glTranslatef(x,y,0);
      ball();
      glPopMatrix();
      
 /*leg*/
  glPushMatrix();
      glTranslatef(lx,ly,0);
      glRotatef(ang, 0.0, 0.0, 1.0);
      leg();
      glPopMatrix();
 /*toe*/
  glPushMatrix();
      glTranslatef(tx,ty,0);
      toe();
      glPopMatrix();
      
 /*wall R*/
  glPushMatrix();
      glFlush();
      glColor3f(1.0,1.0,1.0);
      glBegin(GL_LINE_LOOP);
      glVertex2d(xw,yw);
      glVertex2d(xw,-yw);
      glEnd();
 /*wall L*/
  glPushMatrix();
      glFlush();
      glColor3f(1.0,1.0,1.0);
      glBegin(GL_LINE_LOOP);
      glVertex2d(-xw,yw);
      glVertex2d(-xw,-yw);
      glEnd();
 /*wall U*/
  glPushMatrix();
      glFlush();
      glColor3f(1.0,1.0,1.0);
      glBegin(GL_LINE_LOOP);
      glVertex2d(xw,yw);
      glVertex2d(-xw,yw);
      glEnd();
 /*wall D*/
  glPushMatrix();
      glFlush();
      glColor3f(1.0,1.0,1.0);
      glBegin(GL_LINE_LOOP);
      glVertex2d(xw,-yw);
      glVertex2d(-xw,-yw);
      glEnd();
      
      glFlush();
  glPopMatrix();
  glutSwapBuffers();  
}

void simu(void)
{    
     /*回転*/
    ang = ang + angv;

  if ( ang > 360.0 )
      ang = ang - 360.0;
      
      /*角速度計算*/
      w=(angv*PI/180.0)/dt;
      
    /*向き判定*/
    if(Vx>=0){
        f=1;
    }else if(Vx<0){
        f=0;
    }
    
    /*摩擦力*/
    friction=U*G*dt;
    /*ball X軸の変化量計算*/
    mx=Vx*dt;
    x=x+mx;
      /*ball Y軸の速度計算*/
      Vy=Vy-G*dt;
      /*ball Y軸の変化量計算*/
      my=Vy*dt;
      y=y+my;
      /*leg 速度計算*/
      Vl=7*lr*w*2;  //7*lrは円運動の半径距離//
      Vlx=Vl*cos(ang*PI/180.0);
      Vly=Vl*sin(ang*PI/180.0);
      /*toe位置計算*/
      tx=lx+7*lr*cos(ang*PI/180.0-0.5*PI);
      ty=ly+7*lr*sin(ang*PI/180.0-0.5*PI);
      
      /*toeとの当たり判定*/
      disx=fabs(tx-x);
      disy=fabs(ty-y);
      disbl=sqrt(pow(disx,2.0)+pow(disy,2.0));
      if(disbl<=r+lr){
          Vx=(-Vx+Vlx)*(1+el)/(mb/ml+1)+Vx;
          Vy=(-Vy+Vly)*(1+el)/(mb/ml+1)+Vy;
      }
      
      /*壁Rとの当たり判定*/
      if(x+r>=xw){
          Vx=Vx*e*(-1);
          x=xw-r;
      }
      /*壁Lとの当たり判定*/
      if(x-r<=-xw){
          Vx=Vx*e*(-1);
          x=-xw+r;
      }
      /*壁Uとの当たり判定*/
      if(y+r>=yw){
          Vy=Vy*e*(-1);
          y=yw-r;
      }
      /*壁Dとの当たり判定*/
      if(y-r<=-yw){
          Vy=Vy*e*(-1);
          y=-yw+r;
          if(f==1)
          Vx=Vx-friction;
          if(f==0)
          Vx=Vx+friction;
      }
      
      /*ball停止判定*/
      if(f==1){
          if(Vx<=0)
          mx=0;
      }else if(f==0){
          if(Vx>0)
          mx=0;    
      }
    if((mx>=-0.00009) && (mx<=0.00009))
    mx=0;
    if((my>=-0.00009) && (my<=0.00009))
    my=0;
    
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
  glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
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
  switch(key){
      case 'a':
          lx=lx-sens;
          break;
      case 'd':
          lx=lx+sens;
          break;
      case 'w':
          ly=ly+sens;
          break;
      case 's':
          ly=ly-sens;
          break;
      case 'j':
          lx=tmpx;
          ly=tmpy;
          break;
      case 'c':
          angv=angv-0.05;
          break;
      case 'v':
          angv=angv+0.05;
          break;
      case 'b':
          angv=1.0;
          break;
      case 'n':
          angv=0.0;
          break;
      case 'm':
          ang=0.0;
          angv=1.0;
      default:
          break;
  }
}

int main(int argc, char *argv[])
{
  tmpx=lx;
  tmpy=ly;
  tx=lx;
  ty=ly-7*lr;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (800, 800);
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