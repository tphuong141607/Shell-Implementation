%{
#include "sushi.h"
  int yylex();
  void yyerror(const char* s);  
%}

%union {
  char *s;
  int i;
}

%token YY_SUSHI_SET
%token YY_SUSHI_HISTORY
%token YY_SUSHI_EXIT
%token YY_SUSHI_JOBS
%token YY_SUSHI_PWD
%token YY_SUSHI_CD
%token YY_SUSHI_UNKNOWN
%token YY_SUSHI_LESS
%token YY_SUSHI_MORE
%token YY_SUSHI_MOREMORE
%token YY_SUSHI_AMP
%token YY_SUSHI_BAR
%token<s> YY_SUSHI_TOK

%type<s> arg
%type<i> bgmode

%start cmdline

%%

cmdline: 
  /* an empty line is valid, too! */
| inout_program bgmode /* TODO */
| in_program pipe bgmode /* TODO */

inout_program: 
  program /* TODO */
| program inout_redirection /* TODO */

in_program: 
  program /* TODO */
| program in_redirection /* TODO */

out_program: 
  program /* TODO */
| program out_redirection /* TODO */

inout_redirection: 
  in_redirection out_redirection /* TODO */
| out_redirection in_redirection /* TODO */

out_redirection: 
  YY_SUSHI_MORE arg /* TODO */
| YY_SUSHI_MOREMORE arg /* TODO */

in_redirection: 
  YY_SUSHI_LESS arg /* TODO */

bgmode: /* Done */
  { $$ = 0; }
| YY_SUSHI_AMP { $$ = 1; }

program: 
  args /* TODO */
| arg YY_SUSHI_SET arg  /* TODO */
| YY_SUSHI_HISTORY { /* Add your code here */ }
| YY_SUSHI_EXIT { /* Add your code here */ }
| YY_SUSHI_JOBS /* TODO */
| YY_SUSHI_PWD  /* TODO */
| YY_SUSHI_CD arg /* TODO */

pipe: 
  YY_SUSHI_BAR out_program /* TODO */
| YY_SUSHI_BAR program pipe /* TODO */

args: 
  arg /* TODO */
| args arg /* TODO */

arg: /* Done */
  YY_SUSHI_TOK { $$ = $1; }

%%

/* This section is empty */
