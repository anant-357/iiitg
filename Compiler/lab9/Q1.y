%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
extern int yyparse();
void yyerror(const char *s);
extern FILE* yyin;
char temp[15][10];
int i=0;

%}

%union {
    char *str;
}

%token <str> IF INT FLOAT ELSE WHILE TRUE FALSE A B GT LT EQUAL NOT_EQUAL MOD ID PLUS MINUS TIMES RET DIVIDE COMMA ASSIGN OR AND NOT LP RP RELOP LB RB COLON MAIN '\n'
%type <str> S state contd while exp exp1

%%

S: INT MAIN LP RP LB state { 
        
        int j=7;
        
        printf("%s = %s \n",temp[4],temp[5]);
        printf("%s = %s\n",temp[6], temp[7]);
        printf("a = %s \n",temp[2]);
        printf("b = %s\n\n",temp[3]);
        printf("X: if b != 0 goto Z\n");
        printf("Y: goto W\n");
        printf("Z: %s = b \n",temp[1]);
        printf("    t1 = a %% b \n"); 
        printf("    b = t1 \n");
        printf("    a = %s\n",temp[1]);
        printf("    goto X\n");
        printf("W: %s = a\n", temp[0]);
        
        
        return 0;
    }

    ;

state: INT ID ASSIGN ID COMMA ID ASSIGN ID COLON contd
	{       strcpy(temp[i++], $2);
        	strcpy(temp[i++], $4);
                strcpy(temp[i++], $6);
                strcpy(temp[i++], $8);
          
        } 
        | INT ID ASSIGN ID COLON  INT ID ASSIGN ID COLON contd
	{
		strcpy(temp[i++], $2);
        	strcpy(temp[i++], $4);
                strcpy(temp[i++], $7);
                strcpy(temp[i++], $9);
        }
        
       
       ;

contd: INT A ASSIGN ID COMMA B ASSIGN ID COLON while
       {
          strcpy(temp[i++], $4);
          strcpy(temp[i++], $8);
        
       }
       ;

while: WHILE LP B compare ID RP LB exp{
       
      }
      
	;

exp:  INT ID ASSIGN B COLON B ASSIGN A MOD B COLON A ASSIGN ID COLON RB exp1{
      strcpy(temp[i++], $2);
      
      }
      ;

exp1: INT ID ASSIGN A COLON RET ID COLON RB{
      strcpy(temp[i++], $2);
      
      };


compare: NOT_EQUAL | GT | LT | EQUAL ;

%%

int main() {
    FILE* fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }
     yyin = fp;
    yyparse();
    fclose(fp);
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}




 
