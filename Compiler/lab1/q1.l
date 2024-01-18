%{
  #include <stdio.h>
%}

delimiter [" "|\n]
article "a "|"A "|"an "|"An "|"the "|"The "
                               
%%
{delimiter}{article}+ ECHO;
.|\n ;

%%

int main () {
    yylex();
    return 1;
  }
