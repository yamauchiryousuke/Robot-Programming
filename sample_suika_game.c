#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>

#define PI 3.1415926  //円周率//
#define G 9.80655  //重力加速度//
#define U 0.004  //摩擦係数//
#define e 0.8  //wall反発係数//
#define el 0.8  //leg反発係数//
#define el_ball 0.8  //ball反発係数//

char ic[12];
char str_ball_num[3];
char str_ball_id[3];

int i;
int f;  //向き判定　右：0左：1//
int n;  //ballの番号//
int ball_num = 0; //ballの数//
int paku=0; //パックマン口の開閉フラグ//

double paku_t; //パックマン口の開閉時間カウント//
double paku_t_max=50; //パックマン口の開閉設定時間//

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
double w;  //足角速度//

double Vx=60,Vy=20;  //ball初速度(m/s)//
double Vl,Vlx,Vly; //leg速度(m/s)//

double friction;  //摩擦力//
double xw=95,yw=95;  //壁//

double sens=3;

static GLfloat ang = 0.0, angv=1.0;

/*ballの構造体*/
struct ball_test{
    /* data */
    int active;  //有効1、無効0//
    int size;  //ballの大きさ//
    int f;  //向き判定　右：0左：1//
    double x,y;  //ball原点//
    double ang;  //ball角度//
    double r;  //ball半径//
    double bx,by;  //ball側//
    double mb;  //ball質量//
    double mx,my;  //位置変化量//
    double mang;  //角度変化量//
    double disx,disy,disbl;  //2つの原点との差//
    double dt;  //時間変化量(s)//
    double w;  //角速度//
    double Vx,Vy;  //ball初速度(m/s)//
};

struct ball_test balls[50];

void ball(int n){
    glBegin(GL_POLYGON);
    glColor4f(1.0,1.0,0.0,0.0);
    /*ballの側*/
    glVertex2d(0,0);
    for(i=0;i<=36;i++){
        balls[n].bx=balls[n].r*cos(10*i*PI/180.0);
        balls[n].by=balls[n].r*sin(10*i*PI/180.0);
        glVertex2d(balls[n].bx,balls[n].by);
    }
    glEnd();
}

void ball1(int n){
    glBegin(GL_POLYGON);
    glColor4f(1.0,1.0,0.0,0.0);
    /*ballの側*/
    glVertex2d(0,0);
    for(i=1;i<=35;i++){
        balls[n].bx=balls[n].r*cos(10*i*PI/180.0);
        balls[n].by=balls[n].r*sin(10*i*PI/180.0);
        glVertex2d(balls[n].bx,balls[n].by);
    }
    glEnd();
}

void ball2(int n){
    glBegin(GL_POLYGON);
    glColor4f(1.0,1.0,0.0,0.0);
    /*ballの側*/
    glVertex2d(0,0);
    for(i=2;i<=34;i++){
        balls[n].bx=balls[n].r*cos(10*i*PI/180.0);
        balls[n].by=balls[n].r*sin(10*i*PI/180.0);
        glVertex2d(balls[n].bx,balls[n].by);
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

void removeAndShift(int size, int index1, int index2) {
    int i;
    // 最初の要素を削除
    for (i = index1; i < size - 1; i++) {
        balls[i] = balls[i + 1];
    }
    // インデックスがずれるので、必要に応じて調整
    if (index2 > index1) {
        index2-=index2-index1;
    }
    // 二番目の要素を削除
    for (i = index2; i < size - 2; i++) {
        balls[i] = balls[i + 1];
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (ball_num>0){
        for (int n=0;n<ball_num;n++){
            if(balls[n].active==1){
                glPushMatrix();
                //glClear(GL_COLOR_BUFFER_BIT);
                glEnable(GL_LINE_STIPPLE);
                glLineStipple(1,0xACF3);
                glTranslatef(balls[n].x,balls[n].y,0);
                glRotatef(balls[n].ang, 0.0, 0.0, 1.0);
                if(paku==0||paku==4){
                    ball(n);
                }else if(paku==1||paku==3){
                    ball1(n);
                }else if(paku==2){
                    ball2(n);
                }
                glColor3f(0.0+(balls[n].size/10), 1.0, 1.0);

                glRasterPos2f(0, 0);
                snprintf(str_ball_id, 3, "%d", balls[n].size);
                display_string(str_ball_id, GLUT_BITMAP_8_BY_13);

                glPopMatrix();
                //printf("%.1f",balls[0].x);
            }
        }
    }

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

    if(paku==0&&paku_t==paku_t_max){
        paku=1;
        paku_t=0;
    }else if(paku==1&&paku_t==paku_t_max){
        paku=2;
        paku_t=0;
    }else if(paku==2&&paku_t==paku_t_max*6){
        paku=3;
        paku_t=0;
    }else if(paku==3&&paku_t==paku_t_max){
        paku=4;
        paku_t=0;
    }else if(paku==4&&paku_t==paku_t_max){
        paku=0;
        paku_t=0;
    }
    paku_t++;
    
    /*摩擦力*/
    friction=U*G*dt;
    
    /*------------------構造体sim--------------------*/
    for (int n=0;n<ball_num;n++){
        if(balls[n].active==1){
            balls[n].Vx -= (balls[n].Vx > 0) ? friction : -friction;
            balls[n].Vy-=G*dt;

            // 位置の更新
            balls[n].x += balls[n].Vx * dt;
            balls[n].y += balls[n].Vy * dt;

            // 角速度の更新
            //balls[n].w = friction / balls[n].r * dt;
            balls[n].w = balls[n].Vx*(-10);
            balls[n].mang=balls[n].w*dt;
            balls[n].ang+=balls[n].mang;
            /*ball同士の当たり判定*/
            if(n>0){
                for (int nn=1;nn<=n;nn++){
                    if(balls[n-nn].active==1){
                        disx_b=fabs(balls[n].x-balls[n-nn].x);
                        disy_b=fabs(balls[n].y-balls[n-nn].y);
                        disbb=sqrt(pow(disx_b,2.0)+pow(disy_b,2.0));
                        if(disbb<balls[n].r+balls[n-nn].r){

                            double temp_Vx = balls[n].Vx;
                            double temp_Vy = balls[n].Vy;
                            balls[n].Vx = (balls[n].Vx * (balls[n].mb - balls[n-nn].mb) + 2 * balls[n-nn].mb * balls[n-nn].Vx) / (balls[n].mb + balls[n-nn].mb);
                            balls[n].Vy = (balls[n].Vy * (balls[n].mb - balls[n-nn].mb) + 2 * balls[n-nn].mb * balls[n-nn].Vy) / (balls[n].mb + balls[n-nn].mb);
                            balls[n-nn].Vx = (balls[n-nn].Vx * (balls[n-nn].mb - balls[n].mb) + 2 * balls[n].mb * temp_Vx) / (balls[n].mb + balls[n-nn].mb);
                            balls[n-nn].Vy = (balls[n-nn].Vy * (balls[n-nn].mb - balls[n].mb) + 2 * balls[n].mb * temp_Vy) / (balls[n].mb + balls[n-nn].mb);

                            // 角速度の更新
                            balls[n].w = -balls[n].w;
                            balls[n-nn].w = -balls[n-nn].w;

                            //  同じ大きさのballの結合
                            if(balls[n].size==balls[n-nn].size){
                                if(ball_num<50){
                                    ball_num++;
                                    balls[n+1].active=1;  //有効1、無効0//
                                    balls[n+1].size=balls[n].size+1;  //ball大きさ//
                                    balls[n+1].x=(balls[n].x+balls[n-nn].x)/2,balls[n+1].y=(balls[n].y+balls[n-nn].y)/2;  //ball原点//
                                    balls[n+1].ang=0;  //ball角度//
                                    balls[n+1].r=balls[n].r*2;  //ball半径//
                                    balls[n+1].mb=0.5;  //質量(kg)//
                                    balls[n+1].mx=0,balls[n+1].my=0,balls[n+1].mang=0;  //変化量//
                                    balls[n+1].dt=0.03;  //時間変化量(s)//
                                    balls[n+1].Vx=balls[n].Vx,balls[n+1].Vy=balls[n].Vy;  //ball初速度(m/s)//
                                    balls[n+1].w=balls[n].w;  //ball初角速度(rad/s)//
                                    balls[n].active=0;
                                    balls[n-nn].active=0;
                                    //int array_size = ball_num;
                                    //removeAndShift(array_size, n-nn, n);
                                    //ball_num -= 2;
                                }
                            }

                        }

                    }
                }
            }
            
            /*壁Rとの当たり判定*/
            if(balls[n].x+balls[n].r>=xw){
                balls[n].Vx=balls[n].Vx*e*(-1);
                balls[n].x=xw-balls[n].r;
                balls[n].w = -balls[n].w;
            }
            /*壁Lとの当たり判定*/
            if(balls[n].x-balls[n].r<=-xw){
                balls[n].Vx=balls[n].Vx*e*(-1);
                balls[n].x=-xw+balls[n].r;
                balls[n].w = -balls[n].w;
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
        
        }
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
        case 'f':
            if(ball_num<50){
                ball_num++;
                n = ball_num-1;
                balls[n].active=1;  //有効1、無効0//
                balls[n].size=0;  //ball大きさ//
                balls[n].x=-30,balls[n].y=30;  //ball原点//
                balls[n].ang=0;  //ball角度//
                balls[n].r=5;  //ball半径//
                balls[n].mb=1;  //質量(kg)//
                balls[n].mx=0,balls[n].my=0,balls[n].mang=0;  //変化量//
                balls[n].dt=0.03;  //時間変化量(s)//
                balls[n].Vx=60,balls[n].Vy=20;  //ball初速度(m/s)//
                balls[n].w=-5;  //ball初角速度(rad/s)//
            }
            break;

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