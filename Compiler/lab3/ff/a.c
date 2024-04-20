#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>

#define PRODUCTION_LENGTH 10
#define N_RULES 10
#define N_RETURN 20
#define DEBUG 0

struct rule{
  char symbol[2];
  char produces[PRODUCTION_LENGTH];
  int start;
};

void getString(int pos, char string[], char sub[])
{
    int c = 0;
    char substring[PRODUCTION_LENGTH];
 
    while (c+pos < strlen(string)) {
        substring[c] = string[pos + c - 1];
        c++;
    }
 
    substring[c] = '\0';
    strcpy(sub, substring);
    return 0;
}


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



char* first(struct rule rules[N_RULES], char symbol){
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
  
  struct rule check;
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
      int j = 1;
      if(is_non_terminal(check.produces[i+j])){
        char* f = first(rules, check.produces[i+j]);
                        while(contains(f, '!') && check.produces[i+j]!='|' && check.produces[i+j]!='\0'){                         
                          add_to_set(ret, f);
                          j++;
                          f = first(rules, check.produces[i+j]);
            }
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

char* follow(struct rule rules[N_RULES], char symbol){
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
    char* sub = (char *)malloc(N_RETURN* sizeof(char));
    strcpy(sub, rules[i].produces);
    int index = contains(sub, symbol);
    while(index != -1){
      if(rules[i].produces[index+1] == '\0' || rules[i].produces[index+1] == '|' || rules[i].produces[index+1] == ' '){
        if(rules[i].symbol[0] != symbol){
  add_to_set(ret, follow(rules, rules[i].symbol[0]));
        }
        
      }else{
        char f[N_RETURN];
        int k = 1;
        strcpy(f,first(rules, rules[i].produces[index+k]));
        while(contains(f, '!') && rules[i].produces[index+k] != '|' && rules[i].produces[index+k] != '\0' ){
          add_to_set(ret,f);
          k++;
          strcpy(f, first(rules, rules[i].produces[index+k]));
        }
        if(rules[i].produces[index+k] == '|' || rules[i].produces[index+k] == '\0')
          add_to_set(ret, follow(rules, rules[i].symbol[0]));
      }
      int index = contains(sub, '|');
      if(index == -1)
        break;
      getString(index,0,sub, sub);
      index = contains(sub, symbol);
    }
  }

  return ret;

}


int main(){
  FILE *in;
  in = fopen("productions","r");
  if(in == NULL)
    printf("Unable to open file");

  struct rule rules[N_RULES];
  int i = 0;
  
  while(fscanf(in,"%[^-]->%[^\n]",rules[i].symbol, rules[i].produces) == 2){
    i++;
    fscanf(in, "\n");
  }
  print_rules(rules);
  for(int i=0; i< N_RULES; i++){
    if(strcmp(rules[i].symbol,"") == 0)
      break;
    printf("\nfirst %s: %s \n",rules[i].symbol, first(rules, rules[i].symbol[0]));
    printf("\nfollow %s: %s \n",rules[i].symbol, follow(rules, rules[i].symbol[0]));
  }
  //printf("\n follow %c: %s \n", 'F', follow(rules, 'F'));
  /* char abc[N_RETURN] = "a12e"; */
  /* char add[N_RETURN] = "13ar"; */
  /* add_to_set(abc,add); */
  /* printf("%s", abc); */
  fclose(in);
}
