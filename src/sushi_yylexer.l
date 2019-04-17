%{
#include "sushi.h"
#include "sushi_yyparser.tab.h"
#ifdef __APPLE__
extern int yylex(void);
extern int yyparse(void);
#endif
%}

%option noyywrap

digit         [0-9]
letter        [a-zA-Z_]
alnum         {digit}|{letter}
string        \"([^\n\\\"]|\\.)*\"
quoted_string \'([^\n\\\']|\\.)*\'
permitted     {digit}|{letter}|[-%@_+\[\]\\/,\.:]
unsupported   [;()*^?~{}`]

%%
"#".*"\n"?      { /* comment */ }

exit            { return YY_SUSHI_EXIT; }
pwd             { return YY_SUSHI_PWD;  }
cd              { return YY_SUSHI_CD;   }

\${alnum}+      { yylval.s = super_strdup(sushi_safe_getenv(yytext + 1));
                  return YY_SUSHI_TOK; }
{permitted}+    { yylval.s = super_strdup(sushi_unquote(yytext));
                  return YY_SUSHI_TOK; }
{quoted_string} { yytext[strlen(yytext) - 1] = 0;
                  yylval.s = super_strdup(yytext + 1);
                  return YY_SUSHI_TOK; }
{string}        { yytext[strlen(yytext) - 1] = 0;
                  yylval.s = super_strdup(sushi_unquote(yytext + 1));
                  return YY_SUSHI_TOK; }

"<"             { return YY_SUSHI_LESS;     }
">"             { return YY_SUSHI_MORE;     }
">>"            { return YY_SUSHI_MOREMORE; }
"&"             { return YY_SUSHI_AMP;      }
"|"             { return YY_SUSHI_BAR;      }
"="             { return YY_SUSHI_SET;      }
"!"             { return YY_SUSHI_HISTORY;  }

{unsupported}   { fprintf(stderr, "Unsupported command: %c\n", yytext[0]);
                  return YY_SUSHI_UNKNOWN;}
[\t\n\b\v ]+    { /* whitespaces */ }

.               { fprintf(stderr, "Illegal character: %c\n", yytext[0]);
                  return YY_SUSHI_UNKNOWN; }

%%

// This function will be modified in the future
// The function returns 0 if there are no syntax errors and 1, otherwise
int sushi_parse_command(char *command) {
  YY_BUFFER_STATE buffer = yy_scan_string(command);
  int retval = yyparse();
  yylex_destroy();
  return retval;
}
