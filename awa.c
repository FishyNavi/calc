#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>


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


void pushDouble(DoubleStack *s, double db) {
    s->data[++(s->size)] = db; }

double popDouble(DoubleStack *s) {
    return s->data[(s->size)--]; }

double getDouble(DoubleStack *s) {
    if (s->size == -1) return '\0';
    else return s->data[s->size]; }


void pushChar(CharStack *s, char ch) {
    s->data[++(s->size)] = ch; }

char popChar(CharStack *s) {
    return s->data[(s->size)--]; }

char getChar(CharStack *s) {
    if (s->size == -1) return '\0';
    else return s->data[s->size]; }

int isop(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

struct func {
    char c;
    double (*f)(double); // function pointer
    char n[5]; // name 
};
typedef struct func Func;
double factorial(double x) {
    return tgamma(x+1);
}
double sgn(double x) {
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}
double mathFunction(Func fu,double x) {
    return fu.f(x);
}

struct func functions[12] = {
    {'!',factorial ,"!"},
    {'s',sin,"sine"},
    {'S',asin,"asin"},
    {'c',cos,"cosi"},
    {'C',acos,"acos"},
    {'t',tan,"tan"},
    {'T',atan,"atan"},
    {'e',exp,"expo"},
    {'r',sqrt,"sqrt"},
    {'R',cbrt,"cbrt"},
    {'a',fabs,"absl"},
    {'A',sgn,"sign"},

};


int getFuncIdx(Func *func, char f) {
    for (int i=0; i<12; i++) {
        if (func[i].c==f) {
            return i;
        }
    }
    return 0;
}


int isfunc(char c) {
    
    switch (c) {
        case '!': // factorial
        case 's': // sin
        case 'S': // arcsin
        case 'c': // cos
        case 'C': // arccos
        case 't': // tan
        case 'T': // arctan
        case 'e': // exponential e^x
        case 'r': // square root
        case 'R': // cubic root
        case 'a': // absolute
        case 'A': // sign
            return 1;
        default:
            return 0;


        }
    }
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^')              return 3;
    if (isfunc(op))             return 4;
    return 0;
}

double math(double x,double y,char op) {
    switch (op) {
        case '+': return x+y;
        case '-': return x-y;
        case '*': return x*y;
        case '/': return x/y;
        case '^': return pow(x, y);
    }
}
void handleOps(CharStack *c, DoubleStack *d) {
    double x;
    double y;
    if (isfunc(getChar(c))) {
        
        x = popDouble(d);
        int idx = getFuncIdx(functions,popChar(c));
        pushDouble(d, mathFunction(functions[idx],x));
        
    } 
    else {    
        if (precedence(getChar(c))==1 && d->size == 0) {
            y = popDouble(d); 
            x = 0;
        }
        else if (d->size == 0) {
            printf("%c", getChar(c));
            exit(1); /// change this to sth better in future maybe
        }
        else {
            y = popDouble(d); 
            x = popDouble(d); 
        }
        char op = popChar(c);
        pushDouble(d, math(x,y,op));
    }
}

double calc(char *expr) {
    CharStack ops = { .size = -1};
    DoubleStack vals = { .size = -1};
    int possible_unary = 1;
    int sign = 1;
    int i = 0;
    while (expr[i]!='\0') {
        if (expr[i] == ' ') {
            i++;
            continue;
        }
        if (isdigit(expr[i])) {
            char *end;
            double val = strtod(&expr[i],&end);
            val*=sign;
            sign = 1;
            pushDouble(&vals,val);
            i = end - expr;
            possible_unary = 0;
            continue;
        }
        else if (expr[i]=='(') {
            pushChar(&ops,expr[i]); 
            possible_unary = 1;
        }
        else if (expr[i]==')') {

            while (vals.size!=-1 && ops.size != -1 && getChar(&ops) != '(' ) {
                handleOps(&ops,&vals);
            }
            popChar(&ops);
            possible_unary = 0;
            

        }
        else if (isop(expr[i]) || isfunc(expr[i])) {
            
            if (expr[i] == '-' && possible_unary) {
                sign *= -1;
                i++;
                continue;
            }
            if (expr[i] == '+' && possible_unary) {
                i++;
                continue;
            }
            while (ops.size != -1 && vals.size!=-1 && precedence(getChar(&ops)) >= precedence(expr[i])) {
                
                handleOps(&ops,&vals);
            }
            pushChar(&ops, expr[i]);
            possible_unary = 1;
            
        }
        else if (expr[i]=='p') {

            pushDouble(&vals,sign*M_PI);
            possible_unary = 0;
        }
        i++;

    }
    while (ops.size != -1) {
        handleOps(&ops,&vals);
    }
    return popDouble(&vals);
}

void show(char equation[],int pos) {
    int len = strlen(equation);
    int j = 1;
    for ( int i = 0; i<len;i++){
        if (isfunc(equation[i])) {
            int idx = getFuncIdx(functions,equation[i]);
            printf("%s", functions[idx].n);
            if (isfunc(equation[i]) && equation[i] != '!') {
                j+=3;
            }
        }
        else if (equation[i]=='p') {
            printf("%s","π");
        }
        else printf("%c", equation[i]);
    }
    gotoxy(pos+j,1);
}

void insertChar(char *str, char c, int *pos) {
    int len = strlen(str);
    int j = 0;
    char tmp[len+2];
    
    for(int i=0;i<len;i++) {
        if (i == *pos)
            tmp[j++] = c;
        
        tmp[j++] = str[i];
    }
    if (*pos >= len) 
        tmp[j++] = c;
    
    tmp[j] = '\0'; 
    strcpy(str,tmp);
    (*pos)++;
}
void removeChar(char *str, int *pos) {
    int len = strlen(str);
    for (int i = (*pos-1);i<len;i++) {
        str[i] = str[i+1];
    }
    (*pos)--;
}
int main(int argc, char **argv)
{   
    char *expr = argv[1];
    char str[1256] = "";
    int pos = 0;
    system("clear");
    system("stty raw"); 
    do{

        
        char key = getchar();
        
        switch(key) {
            case 27:
                system("stty cooked");
                system("clear");
                exit(0);
            case 'o':
                if (strlen(str)>pos)
                    pos++;
                break;
            case 'i':
                if (pos>0)
                    pos--;
                break;
            
            default: 
                if (key == 127 && pos>0) {
                    removeChar(str, &pos);
                    if (pos > 0 && isfunc(str[pos - 1])) { 
                        removeChar(str, &pos); 
                    }
                }
                else if (isdigit(key) || isop(key) || key == ')' || key == '(' || key == 'p') insertChar(str, key, &pos);
                else if (isfunc(key)) {
                    insertChar(str, key, &pos);
                    insertChar(str, '(', &pos);
                }
        
        }
        system("clear");

        
        show(str,pos);
        if (key == 'O') printf("\n\r= %g",calc(str));
        
        
        
    } while (1);
    system("stty cooked"); 
    return 0;
}