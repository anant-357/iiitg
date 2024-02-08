#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pt.h"

int main(){
  FILE *in;
  in = fopen("productions","r");
  out = fopen("parsing_table", "w");
  if(in == NULL)
    printf("Unable to open productions file");
  if(out == NULL)
    printf("Unable to open parsing_table file");

  rule rules[N_RULES];
  int i = 0;
  
  while(fscanf(in,"%[^-]->%[^\n]",rules[i].symbol, rules[i].produces) == 2){
    i++;
    fscanf(in, "\n");
  }
  fclose(in);

  print_rules(rules);

  pt parsing_table;
  
  

  /* for(int i=0; i< N_RULES; i++){ */
  /*   if(strcmp(rules[i].symbol,"") == 0) */
  /*     break; */
  /*   printf("\nfirst %s: %s \n",rules[i].symbol, first(rules, rules[i].symbol[0])); */
  /*   printf("\nfollow %s: %s \n",rules[i].symbol, follow(rules, rules[i].symbol[0])); */
  /* } */
}
