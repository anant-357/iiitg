%{
#include<stdio.h>
#include <stdlib.h>
int yylex(void);
int yyerror(char *s);
%}

%token NUMBER                     
%left '+' '-'                                            
%left '*' '/'
%%

init_expr: expr { return $$; };

expr: expr '+' expr 
        { $$ = $1 + $3; 
          printf("adding: %d %d\n", $1, $3); 
        };                                         
     |expr '-' expr 
        { $$ = $1 - $3; 
          printf("subtracting: %d %d\n", $1, $3); 
        };
     |expr '*' expr 
        { $$ = $1 * $3; 
          printf("multiplying: %d %d\n", $1, $3); 
        };
     |expr '/' expr 
        { if($3 == 0){
              printf("Division By Zero");return -1;
          };
          $$ = $1 / $3; 
          printf("dividing: %d %d\n", $1, $3); 
        };
     |'-'NUMBER 
        { $$ = -$2; printf("changing sign\n"); };
     |'('expr')' 
        { $$ = $2; printf("brackets\n"); };
     |NUMBER 
     ;
%%

int main()
{
printf("Enter the expression\n");
int ans = yyparse();
printf("\nExpression is valid, ans = %d\n", ans);
exit(0);
}

int yywrap() {
        return 1;
}


int yyerror(char *s)
{
printf("\nExpression is invalid");
exit(0);
}
