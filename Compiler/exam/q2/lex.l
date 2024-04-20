%{
#include "y.tab.h"
%}

%%

"a" {return A;}
"b" {return B;}
"c" {return C;}
"d" {return D;}
[ \t] ;
. ;

%%

int yywrap() {
    return 1;
  }
