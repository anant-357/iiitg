#include <stdio.h>

int fun1(){
  printf("Inside Function 1\n");
  return 4;
}

void main(){
  int x = 10;
  if (x == 5) {
    fun1();
  }else {
    printf("Hello World\n");
  }
}
