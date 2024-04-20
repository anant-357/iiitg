
#define PRODUCTION_LENGTH 10
#define N_RULES 10
#define N_RETURN 20
#define DEBUG 0

typedef struct rule{
  char symbol[2];
  char produces[PRODUCTION_LENGTH];
  int start;
} rule;


void print_rules(rule rules[N_RULES]);

int contains(char string[], char c);

void add_to_set(char set[], char add[]);

void add_char(char set[], char symbol);

int is_non_terminal(char a);
