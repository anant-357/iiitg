%{
  #include <stdio.h>
  int comm_s = 0;
  int comm_m = 0;
%}
            
%%

[/]{1}[/]{1} {
    if (!comm_m)
        comm_s = 1; 
}                      

[/]{1}[*]{1} {
    if (!comm_m && !comm_s)
        comm_m = 1;
}

[*]{1}[/]{1} {
    if (comm_m)
        comm_m = 0;
}

\n+ {
    if (!comm_m && comm_s)      
        comm_s = 0;
    else
       ECHO;
}

. {
  if (!comm_m && !comm_s)
        ECHO;
}

%%

int main() {
    extern FILE *yyin;
    extern FILE *yyout;
    yyin = fopen("input.java", "r");
    yyout = fopen("output.java", "w");
    yylex();
    return 0;
}
