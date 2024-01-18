%{
    #include<stdio.h>
    int n_char = 0;
    int n_words = 0;
    int n_spaces = 0;
    int n_eol = 0;
%}


%%
[a-zA-Z0-9]+ {n_char+=yyleng;n_words++;}
" "+ {n_spaces+=yyleng;}
\n+ {n_eol+=yyleng;}
. ;

%%
int main(){
    extern FILE *yyin;
    yyin = fopen("in","r");
    yylex();
    printf("no. of chars: %d\nno. of words: %d\nno. of spaces: %d\nno. of eols: %d\n",n_char, n_words, n_spaces, n_eol);
    return 0;
}
