#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ff.h"

char* first(rule rules[N_RULES], char symbol){
  if(DEBUG){
    printf("\nfirst: checking: %c", symbol);
    sleep(2);
  }

  char* ret = (char *)malloc(N_RETURN* sizeof(char));
  int reti = 0;

  if(!is_non_terminal(symbol)){
    ret[reti++] = symbol;
    ret[reti] = '\0';
    return ret;
  }
  
  rule check;
  strcpy(check.symbol, "");

  for(int i=0; i<N_RULES; i++){
    if (rules[i].symbol[0] == symbol){
      strcpy(check.symbol, rules[i].symbol);
      strcpy(check.produces, rules[i].produces);
        }
  }

  if(strcmp(check.symbol,"") == 0)
    return "not found";
  
  for(int i=0; i< strlen(check.produces); i++){
    printf("\n %d: %s", i, ret);
    if(i==0){
      if(is_non_terminal(check.produces[i])){
        add_to_set(ret, first(rules, check.produces[i]));
        reti = strlen(ret);
      }
      else{if(contains(ret, check.produces[i]) == -1){
        ret[reti++] = check.produces[i];
        ret[reti] = '\0';
        }
      }
    }
    else if (check.produces[i] == '|'){
      if(is_non_terminal(check.produces[i+1])){
        add_to_set(ret, first(rules, check.produces[i+1]));
        reti = strlen(ret);
      }
      else{
        if(contains(ret, check.produces[i+1]) == -1){
        ret[reti++] = check.produces[i+1];
        ret[reti] = '\0';
        }
      }
    }
  }
  
  return ret;
}

char* follow(rule rules[N_RULES], char symbol){
  if(DEBUG){
    printf("\nfollow: checking: %c", symbol);
    sleep(2);
  }
  int reti = 0;
  char* ret = (char *)malloc(N_RETURN* sizeof(char));
  for(int i=0; i<N_RULES; i++){
    if(rules[i].symbol[0] == symbol && i == 0){
      if(contains(ret,'$') == -1){
        ret[reti++] = '$';
        ret[reti] = '\0';
      }
    }
    int index = contains(rules[i].produces, symbol);
    if(index != -1){
      if(rules[i].produces[index+1] == '\0' || rules[i].produces[index+1] == '|' || rules[i].produces[index+1] == ' '){
        if(rules[i].symbol[0] != symbol){
  add_to_set(ret, follow(rules, rules[i].symbol[0]));
        }
        
      }else{
        char f[N_RETURN];
        strcpy(f,first(rules, rules[i].produces[index+1]));
        char fb[N_RETURN];
        for(int j = 0;j < strlen(f); j++ ){
          
          if(f[j] == '!'){
            strcpy(fb,follow(rules, rules[i].symbol[0]));
            add_to_set(ret, fb);
            reti = strlen(ret);
          }else{
            ret[reti++] = f[j];
            ret[reti] = '\0';
          }
        }
        
      }
    }
  }

  return ret;

}
