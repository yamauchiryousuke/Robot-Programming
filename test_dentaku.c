#include<stdio.h>
#include<math.h>

int main(){
    double element0, element1, element2;
    double a, b, c;
    void f_addition(double*,double*,double*);
    void f_subtraction(double*,double*,double*);
    void f_multiplication(double*,double*,double*);
    void f_division(double*,double*,double*);
    /////input/////
    double num0;
    scanf("%lf",&num0);
    element0 = num0;
    
    char snb0,snb;
    scanf("%s",&snb0);

    int flag;
    flag=1;

    snb = snb0;

    while(flag==1){

        double num1;
        scanf("%lf",&num1);
        element1 = num1;

        switch(snb){
            case '+':
                /////addition/////
                f_addition(&element0,&element1,&element2);
                break;
            case '-':
                /////subtraction/////
                f_subtraction(&element0,&element1,&element2);
                break;
            case '*':
                /////multiplication/////
                f_multiplication(&element0,&element1,&element2);
                break;
            case '/':
                /////division/////
                f_division(&element0,&element1,&element2);
                break;
            default:
                break;
        }
        element0 = element2;

        char snb2;
        scanf("%s",&snb2);
        snb = snb2;

        if(snb==('=')){
            flag=0;
        }
        /////output/////
        fprintf(stdout,"%.3lf\n",element2);

        return(0);
    }
}

void f_addition(double *a, double *b, double *c){
    (*c) = (*a)+(*b);
}
void f_subtraction(double *a, double *b, double *c){
    (*c) = (*a)-(*b);
}
void f_multiplication(double *a, double *b, double *c){
    (*c) = (*a)*(*b);
}
void f_division(double *a, double *b, double *c){
    (*c) = (*a)/(*b);
}