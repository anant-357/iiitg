%{
  #include <stdio.h>
%}

delimiter [" "|\n]
article "a "|"A "|"an "|"An "|"the "|"The "
verb [a-z]+"ing" 

                               
%%
[\t]+
{delimiter}{article} printf("article:%s",yytext);
{verb} printf("verb:%s", yytext);
.|\n ;

%%

int main () {
    yylex();
    return 1;
  }
