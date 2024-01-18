%{
    #include<stdio.h>
%}

small [a-z]

%%
{small} ECHO;
. ;

%%
int main(){
    yylex();
    return 0;
}
