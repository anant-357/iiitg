%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE* yyin;
int temporaryGenerated = 0;
int yydebug=1;

struct quadruple {
    int line;
    char* op;
    char* arg1;
    char* arg2;
    char* result;
} tuple[1000];

int tupleIndex = 0;
int lineCount = 1;

void emit(char* op, char* arg1, char* arg2, char* result) {
    tuple[tupleIndex].line = lineCount;
    tuple[tupleIndex].op = op;
    tuple[tupleIndex].arg1 = arg1;
    tuple[tupleIndex].arg2 = arg2;
    tuple[tupleIndex].result = result;
    tupleIndex++;
    lineCount++;
}

char* newTemp() {
    temporaryGenerated++;
    char* temp;
    return temp;
}

void printCode() {
    for (int i=0; i <= tupleIndex - 1; i++) {
        printf("%d:\t%s %s %s %s\n", tuple[i].line, tuple[i].result, tuple[i].arg2, tuple[i].op, tuple[i].arg1);
    }
}
int yylex();
int yyerror(char* s);

%}

%union {
    char* sval;
}

%token <sval> IDENTIFIER STRING_LITERAL FLOATNUM INTEGER INCLUDE_STMT
%token INT FLOAT LP RP SEMICOLON ASSIGN PRINTF COMMA MULT  LB RB DIV
%type <sval>  Function Statement Expression

%%

Program: Include Functions { printCode(); }
;

Include: INCLUDE_STMT Include {printf("Here");}
       | INCLUDE_STMT 
       ;


Functions: Function Functions { emit($1,"", "", "");}
         | Function
         ;

Function: Typed_Identifier LP Args RP LB Statements RB { $$ = "Function 1";};


Args: Typed_Identifier COMMA Args 
    | ;


Statements: Declaration Statements {};


Declaration: Typed_Identifier Declaration {}
           | Typed_Identifier SEMICOLON
           ;

Typed_Identifier: Type IDENTIFIER ;

Statements: Statement Statements
          | Statement;

Statement: IDENTIFIER ASSIGN Expression SEMICOLON { emit("=", $3, "", $1); }
//         | PRINTF LP STRING_LITERAL COMMA Expression RP SEMICOLON { emit("printf", $5, "", ""); }
//         | Identifier ASSIGN Expression MULT Expression MULT Expression SEMICOLON {char* t1 = newTemp(); emit("*", $3, $5, t1); $$ = newTemp(); emit("*", t1, $7, $$); emit("=", $$, "", $1);  }
//         | Identifier ASSIGN Expression DIV Expression SEMICOLON {$$ = newTemp(); emit("/",$3,$5,$$); emit("=",$$,"",$1);}
//         
//         ;
//
Expression: IDENTIFIER {}
          | INTEGER {}
          | FLOATNUM {}
          | LP Expression RP { $$ = $2; }
//          ;
//
Type: INT {}
    | FLOAT {printf("float");}
    ;





%%

int yyerror(char* s) {
    printf("%s\n", s);
}

int main() {
    FILE* fp = fopen("interest.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }
    yyin = fp;
    yyparse();
    fclose(fp);
    return 0;
}
