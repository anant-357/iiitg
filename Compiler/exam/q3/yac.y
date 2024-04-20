%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern FILE* yyin;
int yylex();
int yyerror(char *s);
int yydebug = 1;
%}

%token SELECT IDENTIFIER FROM WHERE INTEGER FLOAT ADD SUB MUL DIV EQUAL GT LT COMMA INV_COMMA OR AND LP RP;

%%

Start: SELECT Identifiers FROM IDENTIFIER Where {printf("SQL select statement consist by correct arithmetic expression.");};

Identifiers: IDENTIFIER COMMA Identifiers 
           | IDENTIFIER 
           | MUL;

Where: WHERE Comparisons 
     | ;

Comparisons: Comparison Log_Op Comparisons
           | Comparison;

Comparison: IDENTIFIER Compare INV_COMMA Expression INV_COMMA;

Log_Op: OR 
      | AND;

Compare: EQUAL 
       | GT Equal 
       | LT Equal;

Equal: EQUAL | ;

Expression: Expression Arith_Op Expression
          | LP Expression RP
          | Token ;

Arith_Op: ADD | SUB | MUL | DIV ;

Token: IDENTIFIER
     | FLOAT
     | INTEGER ;

%%

int yyerror(char* s) {
    printf("SQL select statement is incorrect or consists by incorrect arithmetic expression.\n");
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
