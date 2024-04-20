%{
    #include "y.tab.h"
    #include <string.h>
    #include <stdio.h>
    int yyerror(const char *);
%}

%%
[ ]        ;
"noun"      { yylval.str = strdup(yytext); return NOUN; }
"pronoun"   { yylval.str = strdup(yytext); return PRONOUN; }
"verb"      { yylval.str = strdup(yytext); return VERB; }
"adjective" { yylval.str = strdup(yytext); return ADJECTIVE; }
[.]         {return FULLSTOP;}
\n         {return '\n';}
%%

int yyerror(const char *s) {
    printf("Expression is not well-formed\n");
    exit(1);
}
