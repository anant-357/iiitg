/* declaration section in this sections we will declare the different value and include the header file which we are using in this program to run this program */

%{
#include "y.tab.h"
extern int yylval;
%}

/* defined section */

%%
[0-9]+ {yylval=atoi(yytext); return NUMBER;}   //this is send to the yacc code as token INTEGER
[\t]+ ;
\n {return 0;}
. {return yytext[0];}
%%
