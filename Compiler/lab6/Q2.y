%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *);
int yylex(void);
%}

%union
{
    char str;
};

%type <str> SUBJECT
%token <str> NOUN PRONOUN VERB ADJECTIVE FULLSTOP
%token <str> '\n'
%%
LINE:SUBJECT '\n' {
		printf("Nice Sentence\n");
		return 0;
};

SUBJECT:NOUN verb obj FULLSTOP
        {printf("Well formed\n");}
        |PRONOUN verb obj
        {printf("Well formed\n");}
 verb:VERB
 obj:NOUN
    |PRONOUN
    |ADJECTIVE
    ;
 

%%
int yywrap(){
    return 1;
}

int main() {
    printf("Enter the expression:\n");
    yyparse();
    return 0;
}
