#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sushi_yyparser.tab.h"

// https://en.wikipedia.org/wiki/Escape_sequences_in_C#Table_of_escape_sequences
char *sushi_unquote(char * s) {
	int len = strlen(s);
	char *ns = (char*)malloc(sizeof(char*)*len); // Dealing with New String
	char *p = s;
	char *q = ns;
	while (*p !='\0'){
	//      printf("c=%x %d\n",*p,*p);
      if(*p == '\\') {
	if(*(p+1) == 'n') 
	   *q = '\n';
	if(*(p+1) == 'a')
	   *q = 0x07;
	if(*(p+1) == 'b')
	   *q = 0x08;
	if(*(p+1) == 'f')
	   *q = 0x0c;
	if(*(p+1) == 'r')
	   *q = 0x0d;
	if(*(p+1) == 't')
	   *q = 0x09;
	if(*(p+1) == 'v')
	   *q = 0x0b;
	if(*(p+1) == '\\')
	   *q = 0x5c;
	if(*(p+1) == '\'')
	   *q = 0x27;
	if(*(p+1) == '\"')
	   *q = 0x22;
	if(*(p+1) == '?')
	   *q = 0x3f;
	if(*(p+1) == 'e')
	   *q = 0x1b;
	p++;
      }
      else
	*q = *p;
      p++;
      q++;
    }
   *q = '\0';
	
return ns;
}	

// Do not modify this function
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
}
