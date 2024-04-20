%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern FILE* yyin;
int yylex();
int yyerror(char *s);
%}

%token A B C D E

%%

Start: Conflict1 {printf("Completed!");}
//Conflict1: Cn A
//         | A;
//
//Cn : B 
//   ;
//
Conflict1: Cn A
         | A;

Cn : B 
   |
   ;

%%

int yyerror(char* s) {
    printf("%s\n", s);
}

int main() {
    FILE* fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }
    yyin = fp;
    yyparse();
    fclose(fp);
    return 0;
}
