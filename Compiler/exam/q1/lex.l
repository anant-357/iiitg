%{
#include "y.tab.h"
%}

%%

"aaa" {return A;}
"bbb" {return B;}
"ccc" {return C;}
"ddd" {return D;}
"eee" {return E;}
[ \t] ;
. ;

%%

int yywrap() {
    return 1;
  }
