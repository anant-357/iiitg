%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern FILE* yyin;
int yylex();
int yyerror(char *s);
int n = 0;
int m = 0;
%}

%token A B C D 
%%

Start: ad {printf("m:%d, n:%d", m, n);}

ad: A ad D { m++; }
  | bc;

bc: B bc C { n++; }
  | ; 

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
