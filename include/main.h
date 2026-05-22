#ifndef CALCULATOR_H
#define CALCULATOR_H

#include<stdio.h>

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define SIZE 128

typedef struct {
    double data[SIZE];
    int size;
} DoubleStack;

typedef struct {
    char data[SIZE];
    int size;
} CharStack;

struct func {
    char c;
    double (*f)(double); 
    char n[5]; 
};
typedef struct func Func;
extern struct func functions[12];

void pushDouble(DoubleStack *s, double db);
double popDouble(DoubleStack *s);
double getDouble(DoubleStack *s);

void pushChar(CharStack *s, char ch);
char popChar(CharStack *s);
char getChar(CharStack *s);

int isop(char c);
double factorial(double x);
double sgn(double x);
double mathFunction(Func fu, double x);
int getFuncIdx(Func *func, char f);
int isfunc(char c);
int precedence(char op);
double math(double x, double y, char op);
int handleOps(CharStack *c, DoubleStack *d);
double calc(char *expr);
void show(char equation[], int pos);
void insertChar(char *str, char c, int *pos);
void removeChar(char *str, int *pos);

#endif