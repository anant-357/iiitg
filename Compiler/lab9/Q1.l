%{
#include "y.tab.h"
#include <string.h>
%}

%%

[ \t]                   ; // Ignore whitespace
"if" { yylval.str = strdup(yytext); return IF; }
"int" { yylval.str = strdup(yytext); return INT; }
"main" { yylval.str = strdup(yytext); return MAIN; }
"float" { yylval.str = strdup(yytext); return FLOAT; }
"else" { yylval.str = strdup(yytext); return ELSE; }
"while" { yylval.str = strdup(yytext); return WHILE; }
"true" { yylval.str = strdup(yytext); return TRUE; }
"false" { yylval.str = strdup(yytext); return FALSE; }
"a" { yylval.str = strdup(yytext); return A; }
"b" { yylval.str = strdup(yytext); return B; }
"return" { yylval.str = strdup(yytext); return RET; }
"%" { yylval.str = strdup(yytext); return MOD; }
[A-Za-z0-9][A-za-z0-9]* { yylval.str = strdup(yytext); return ID; }
"!=" {yylval.str = strdup(yytext); return NOT_EQUAL; }
"+" { yylval.str = strdup(yytext); return PLUS; }
"-" { yylval.str = strdup(yytext); return MINUS; }
"*" { yylval.str = strdup(yytext); return TIMES; }
"/" { yylval.str = strdup(yytext); return DIVIDE; }
"," { yylval.str = strdup(yytext); return COMMA; }
"=" { yylval.str = strdup(yytext); return ASSIGN; }
"||" { yylval.str = strdup(yytext); return OR; }
"&&" { yylval.str = strdup(yytext); return AND; }
"!" { yylval.str = strdup(yytext); return NOT; }
"(" { yylval.str = strdup(yytext); return LP; }
")" { yylval.str = strdup(yytext); return RP; }
"<="|"=="|">="|"<"|">"|"<>" { yylval.str = strdup(yytext); return RELOP; }
"{" { yylval.str = strdup(yytext); return LB; }
"}" { yylval.str = strdup(yytext); return RB; }
";" { yylval.str = strdup(yytext); return COLON; }


%%

int yywrap() {
    return 1;
}
