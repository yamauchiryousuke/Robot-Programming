#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>

#define PI 3.1415926
#define G 9.80655
#define U 0.004
#define e 0.8  //wall反発係数//
#define el 0.8  //leg反発係数//

char ic[12];
char str_ball_num[3];

int i;
int f;  //向き判定　右：0左：1//
int ball_num = 0; //ballの数//

double x=-30,y=30;  //ball原点//
double r=5;  //ball半径//
double bx,by;  //ball側//

double lx=-50,ly=-50,tmpx,tmpy;  //leg原点//
double tx,ty;  //toe原点//
double lr=4;  //toe半径//
double ltx,lty;  //toe側//

double mb=1,ml=60;  //質量(kg)//

double mx=0,my=0;  //変化量//
double disx,disy,disbl,disx_b,disy_b,disbb;  //2つの原点との差//
double dt=0.03;  //時間変化量(s)//
double w;  //角速度//

double Vx=60,Vy=20;  //ball初速度(m/s)//
double Vl,Vlx,Vly; //leg速度(m/s)//

double friction;  //摩擦力//
double xw=95,yw=95;  //壁//

double sens=3;

static GLfloat ang = 0.0, angv=1.0;

/*ballの構造体*/
struct ball_test{
    /* data */
    int f;  //向き判定　右：0左：1//
    double x,y;  //ball原点//
    double r;  //ball半径//
    double bx,by;  //ball側//
    double mb;
    double mx,my;  //変化量//
    double disx,disy,disbl;  //2つの原点との差//
    double dt;  //時間変化量(s)//
    double w;  //角速度//
    double Vx,Vy;  //ball初速度(m/s)//
};

struct ball_test balls[50];
/*
for(int n=0;n<50;n++){
    balls[n].x=-30,balls[n].y=30;  //ball原点//
    balls[n].r=5;  //ball半径//
    balls[n].mb=1;  //質量(kg)//
    balls[n].mx=0,balls[n].my=0;  //変化量//
    balls[n].dt=0.03;  //時間変化量(s)//
    balls[n].Vx=60,balls[n].Vy=20;  //ball初速度(m/s)//
}
*/
void ball(void){
    glBegin(GL_POLYGON);
    glColor4f(1.0,1.0,0.0,0.0);
    /*ballの側*/
    glVertex2d(0,0);
    for(i=2;i<=34;i++){
        bx=r*cos(10*i*PI/180.0);
        by=r*sin(10*i*PI/180.0);
        glVertex2d(bx,by);
    }
    glEnd();
}

void ball1(void){
    glBegin(GL_POLYGON);
    glColor4f(1.0,1.0,0.0,0.0);
    /*ballの側*/
    glVertex2d(0,0);
    for(i=2;i<=34;i++){
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

void display_string(char *str, void *font)
{
    unsigned int i;
    for(i = 0; i < strlen(str); i++) // 一文字ずつ描画
    glutBitmapCharacter(font, str[i]); // 一文字描画
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    /*ball*/
    /*
    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0xACF3);
    glTranslatef(x,y,0);
    ball();
    glPopMatrix();
    */

    if (ball_num>0){
        for (int n=0;n<ball_num;n++){
            glPushMatrix();
            //glClear(GL_COLOR_BUFFER_BIT);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1,0xACF3);
            glTranslatef(balls[n].x,balls[n].y,0);
            ball1();
            glPopMatrix();
            //printf("%.1f",balls[0].x);
        }
    }
      
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
    //glFlush();
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(xw,yw);
    glVertex2d(xw,-yw);
    glEnd();
    /*wall L*/
    glPushMatrix();
    //glFlush();
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(-xw,yw);
    glVertex2d(-xw,-yw);
    glEnd();
    /*wall U*/
    glPushMatrix();
    //glFlush();
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(xw,yw);
    glVertex2d(-xw,yw);
    glEnd();
    /*wall D*/
    glPushMatrix();
    //glFlush();
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(xw,-yw);
    glVertex2d(-xw,-yw);
    glEnd();

    // 文字列の表示
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos2f(-90, 90);
    snprintf(ic, 12, "%f", balls[1].x);
    display_string(ic, GLUT_BITMAP_8_BY_13);

    glColor3f(0.0, 1.0, 1.0);
    glRasterPos2f(-90, 85);
    snprintf(str_ball_num, 3, "%d", ball_num);
    display_string(str_ball_num, GLUT_BITMAP_8_BY_13);
      
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
    
    /*------------------構造体sim--------------------*/
    for (int n=0;n<ball_num;n++){

        /*向き判定*/
        if(balls[n].Vx>=0){
            balls[n].f=1;
        }else if(balls[n].Vx<0){
            balls[n].f=0;
        }
        
        /*ball X軸の変化量計算*/
        balls[n].mx=balls[n].Vx*dt;
        balls[n].x+=balls[n].mx;
        /*ball Y軸の速度計算*/
        balls[n].Vy-=G*dt;
        /*ball Y軸の変化量計算*/
        balls[n].my=balls[n].Vy*dt;
        balls[n].y+=balls[n].my;
        
        /*toeとの当たり判定*/
        disx=fabs(tx-balls[n].x);
        disy=fabs(ty-balls[n].y);
        disbl=sqrt(pow(disx,2.0)+pow(disy,2.0));
        if(disbl<=balls[n].r+lr){
            balls[n].Vx=(-balls[n].Vx+Vlx)*(1+el)/(balls[n].mb/ml+1)+balls[n].Vx;
            balls[n].Vy=(-balls[n].Vy+Vly)*(1+el)/(balls[n].mb/ml+1)+balls[n].Vy;
        }

        /*ball同士の当たり判定*/
        if(n>0){
            for (int nn=1;nn<=n;nn++){
                disx_b=fabs(balls[n].x-balls[n-nn].x);
                disy_b=fabs(balls[n].y-balls[n-nn].y);
                disbb=sqrt(pow(disx_b,2.0)+pow(disy_b,2.0));
                if(disbb<balls[n].r+balls[n-nn].r){

                    //balls[n].Vx+=(-balls[n].Vx+balls[n-nn].Vx)*(1+el)/(balls[n].mb/balls[n-nn].mb+1);
                    balls[n].Vx=(balls[n].mb*balls[n].Vx+balls[n-nn].mb*balls[n-nn].Vx-el*balls[n-nn].mb*(balls[n].Vx-balls[n-nn].Vx))/(balls[n].mb+balls[n-nn].mb);
                    balls[n-nn].Vx=(balls[n].mb*balls[n].Vx+balls[n-nn].mb*balls[n-nn].Vx+el*balls[n].mb*(balls[n].Vx-balls[n-nn].Vx))/(balls[n].mb+balls[n-nn].mb);
                    //balls[n-nn].Vx-=(-balls[n-nn].Vx+balls[n].Vx)*(1+el)/(balls[n-nn].mb/balls[n].mb+1);
                    balls[n-nn].Vx=-balls[n-nn].Vx;
                    if(balls[n].x>=balls[n-nn].x)
                        balls[n].x+=(disx_b-balls[n].r*disx_b/disbb);
                    else
                        balls[n].x-=(disx_b-balls[n].r*disx_b/disbb);
                    //balls[n].Vy+=(-balls[n].Vy+balls[n-nn].Vy)*(1+el)/(balls[n].mb/balls[n-nn].mb+1);
                    //balls[n-nn].Vy-=(-balls[n-nn].Vy+balls[n].Vy)*(1+el)/(balls[n-nn].mb/balls[n].mb+1);
                    balls[n].Vy=(balls[n].mb*balls[n].Vy+balls[n-nn].mb*balls[n-nn].Vy-el*balls[n-nn].mb*(balls[n].Vy-balls[n-nn].Vy))/(balls[n].mb+balls[n-nn].mb);
                    balls[n-nn].Vy=(balls[n].mb*balls[n].Vy+balls[n-nn].mb*balls[n-nn].Vy+el*balls[n].mb*(balls[n].Vy-balls[n-nn].Vy))/(balls[n].mb+balls[n-nn].mb);
                    balls[n-nn].Vy=-balls[n-nn].Vy;
                    if(balls[n].y>=balls[n-nn].y)
                        balls[n].y+=(disy_b-balls[n].r*disy_b/disbb);
                    else
                        balls[n].y-=(disy_b-balls[n].r*disy_b/disbb);
                }

            }
        }
        
        /*壁Rとの当たり判定*/
        if(balls[n].x+balls[n].r>=xw){
            balls[n].Vx=balls[n].Vx*e*(-1);
            balls[n].x=xw-balls[n].r;
        }
        /*壁Lとの当たり判定*/
        if(balls[n].x-balls[n].r<=-xw){
            balls[n].Vx=balls[n].Vx*e*(-1);
            balls[n].x=-xw+balls[n].r;
        }
        /*壁Uとの当たり判定*/
        if(balls[n].y+balls[n].r>=yw){
            balls[n].Vy=balls[n].Vy*e*(-1);
            balls[n].y=yw-balls[n].r;
        }
        /*壁Dとの当たり判定*/
        if(balls[n].y-balls[n].r<=-yw){
            balls[n].Vy=balls[n].Vy*e*(-1);
            balls[n].y=-yw+balls[n].r;
            if(f==1)
                balls[n].Vx=balls[n].Vx-friction;
            if(f==0)
                balls[n].Vx=balls[n].Vx+friction;
        }
        
        /*ball停止判定*/
        if(balls[n].f==1){
            if(balls[n].Vx<=0)
                balls[n].mx=0;
            }else if(balls[n].f==0){
                if(balls[n].Vx>0)
                    balls[n].mx=0;    
            }
        if((balls[n].mx>=-0.00009) && (balls[n].mx<=0.00009))
            balls[n].mx=0;
        if((balls[n].my>=-0.00009) && (balls[n].my<=0.00009))
            balls[n].my=0;
    
    }

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
    switch (button){
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
            case 'f':
                ball_num++;
                int n;
                n = ball_num-1;
                balls[n].x=-30,balls[n].y=30;  //ball原点//
                balls[n].r=5;  //ball半径//
                balls[n].mb=1;  //質量(kg)//
                balls[n].mx=0,balls[n].my=0;  //変化量//
                balls[n].dt=0.03;  //時間変化量(s)//
                balls[n].Vx=60,balls[n].Vy=20;  //ball初速度(m/s)//
                

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
    glutInitWindowSize (600, 600);
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