%{
#include "y.tab.h"
%}

%%
[ \t]           ; 
"#include <stdio.h>"    {return INCLUDE_STMT;}
"int main() {"          {return INT_MAIN;}
"float"         { return FLOAT; }
"int"           { return INT; }
"printf"        { return PRINTF; }
[0-9]+\.[0-9]+  { yylval.sval = strdup(yytext); return FLOATNUM; }
[0-9]+          { yylval.sval = strdup(yytext); return INTEGER; }
[a-zA-Z][a-zA-Z0-9]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
"="             { return ASSIGN; }
"/"             { return DIV;}
"("             { return LP; }
")"             { return RP; }
"{"             { return LB; }
"}"             { return RB; }
"*"             { return MULT;}
\"([^\\\n"]|\\.)*\" { yylval.sval = strdup(yytext); return STRING_LITERAL; }
","             {return COMMA;}
";"             { return SEMICOLON; }
%%

int yywrap() {
    return 1;
}
