#include <stdio.h>

int main() {
  int num1, num2, greater;

  num1 = 46;
  num2 = -50;

  if (num1 > num2) {
    greater = num1;
  } else {
    greater = num2;
  }

  printf("\nThe greater number is: %d\n", greater);
  return 0;
}
