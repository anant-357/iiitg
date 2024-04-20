%{
#include "y.tab.h"
%}
%%

"'" {return INV_COMMA;}
"," {return COMMA;}
"=" {return EQUAL;}
">" {return LT;}
"<" {return GT;}
"+" {return ADD;}
"*" {return MUL;}
"/" {return DIV;}
"-" {return SUB;}
"(" {return LP;}
")" {return RP;}
[oO][rR] {return OR;}
[aA][nN][dD] {return AND;}
[fF][rR][oO][mM] {return FROM;}
[sS][eE][lL][eE][cC][tT] {return SELECT;}
[wW][hH][eE][rR][eE] {return WHERE;}
[a-zA-Z][a-zA-Z0-9]* { return IDENTIFIER;}
[0-9]+ {return INTEGER;}
[0-9]+\.[0-9]+ {return FLOAT;}
. ;

%%

int yywrap(){
    return 1;
  }
