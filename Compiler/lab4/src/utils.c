#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

void print_rules(struct rule rules[N_RULES]){
  for(int i = 0; i < N_RULES; i++){
    if(strcmp(rules[i].symbol,"") == 0)
      break;
    printf("%s:%s\n", rules[i].symbol, rules[i].produces);
  }
}

int contains(char string[], char c){
  for(int i = 0; i<strlen(string); i++){
    if(string[i] == c)
      return i;
  }
  return -1;
}

void add_to_set(char set[N_RETURN], char add[N_RETURN]){
  char* new = (char *)malloc(N_RETURN* sizeof(char));
  int newi = 0;
  new[newi] = '\0';
  for(int i = 0; i < strlen(add); i++){
    if(contains(set, add[i]) == -1 && contains(new, add[i]) == -1){
      new[newi++] = add[i];
      new[newi] == '\0';
    } 
  }
  strcat(set, new);
}

int is_non_terminal(char a){
  if( a >= 'A' && a <= 'Z')
    return 1;
  return 0;
}
