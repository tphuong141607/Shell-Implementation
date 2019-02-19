#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sushi_yyparser.tab.h"

char *sushi_unquote(char *s) {
    int sINDX = 0;
    int len = strlen(s);
    
    while (s[sINDX] != 0) {
        // If whitespace characters, enter this part of the code.
        if ((s[sINDX] == '\\') && (s[sINDX+1] != 0)) {
            switch(s[sINDX+1]){
                case 'a': s[sINDX] = '\a'; break;
                case 'b': s[sINDX] = '\b'; break;
                case 'f': s[sINDX] = '\f'; break;
                case 'n': s[sINDX] = '\n'; break;
                case 'r': s[sINDX] = '\r'; break;
                case 't': s[sINDX] = '\t'; break;
                case 'v': s[sINDX] = '\v'; break;
                case '\\': s[sINDX] = '\\'; break;
                case '\'': s[sINDX] = '\''; break;
                case '"': s[sINDX] = '\"'; break;
                case '?': s[sINDX] = '\?'; break;
            }
            /* If there is nothing after the whitespace characters,
               simply assign NULL terminator to s[sINdx + 1]
               Example: '\' and 'n' --> '\n' and '\0' */
            if (s[sINDX + 2] == '\0') {
                s[sINDX + 1] = '\0';
                
            } else {
                /* If we still have some characters after the whitespace characters
                   shift all characters starting from sINDX+1 until the end of the string
                   Example: '\' and 'n' and 'a' --> '\n' and 'a' */
                for (int i = sINDX+1; i < len; i++) {
                    s[i] = s[i + 1];
                }
            }
        }
        // If not whitespace characters, simply increment
        sINDX++;
    }
    return s;
}

// Do not modify this function
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
}
