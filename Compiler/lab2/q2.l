%{
  #include <stdio.h>
  FILE *doc;
  int comm_s = 0;
  int comm_m = 0;
  int comm_d = 0;
%}
            
%%

[/]{1}[/]{1} {
    if (!comm_m && !comm_d)
        comm_s = 1; 
}                      

[/]{1}[*]{1} {
    if (!comm_m && !comm_s && !comm_d)
        comm_m = 1;
}

[/]{1}[*]{1}[*]{1} {
    if (!comm_m && !comm_s && !comm_d)
        comm_d = 1;
}

[*]{1}[/]{1} {
    if (comm_m)
        comm_m = 0;
    else if (comm_d){
        comm_d = 0;
        fprintf(doc, "%s", "\n->");
    }
}

\n {
    if (!comm_m && !comm_d && comm_s)      
        comm_s = 0;
    else
        ECHO;
}

. {
  if (!comm_m && !comm_d && !comm_s)
        ECHO;
  else if (comm_d)
      fprintf(doc, "%s", yytext);
}

%%

int main() {
    
  yyout = fopen("output.java", "w");
  doc = fopen("doc.txt", "w");
  fprintf(doc, "%sDoc File", " ");
   fprintf(doc, "%s", "\n->");
  yyin = fopen("input.java", "r");

    yylex();
  return 0;
}
