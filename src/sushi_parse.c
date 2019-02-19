#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sushi_yyparser.tab.h"

char *sushi_unquote(char *s) {
    int len = strlen(s);
    char *newString = malloc((len + 1)); // +1 b/c of NULL
    
    int sINDX = 0;
    int newStringINDX = 0;
    
    while (s[sINDX] != 0) {
        if (s[sINDX] == '\\') {
            switch(s[sINDX+1]){
                case 'a': newString[newStringINDX] = '\a'; break;
                case 'b': newString[newStringINDX] = '\b'; break;
                case 'f': newString[newStringINDX] = '\f'; break;
                case 'n': newString[newStringINDX] = '\n'; break;
                case 'r': newString[newStringINDX] = '\r'; break;
                case 't': newString[newStringINDX] = '\t'; break;
                case 'v': newString[newStringINDX] = '\v'; break;
                case '\\': newString[newStringINDX] = '\\'; break;
                case '\'': newString[newStringINDX] = '\''; break;
                case '"': newString[newStringINDX] = '\"'; break;
                case '?': newString[newStringINDX] = '\?'; break;
            }
            printf("%c", newString[newStringINDX]);
            newStringINDX++;
            sINDX+=2;
            
        } else {
                newString[newStringINDX] = s[sINDX];
                newStringINDX++;
                sINDX++;
        }
    }
    return newString;
}

// Do not modify this function
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
}
