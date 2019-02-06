#include <stdlib.h>
#include <stdio.h>
#include "sushi_yyparser.tab.h"

// https://en.wikipedia.org/wiki/Escape_sequences_in_C#Table_of_escape_sequences
char *sushi_unquote(char * s) {
  return s;
}

// Do not modify this function
int yywrap(void) {
  return 1;
}

// Do not modify this function
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
}
