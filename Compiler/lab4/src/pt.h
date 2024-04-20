#include "ff.h"

typedef struct pt{
  int table[N_RULES][PRODUCTION_LENGTH];
  char columns[PRODUCTION_LENGTH];
  char rows[N_RULES];
} pt;
