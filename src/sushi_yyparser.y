%{
#include "sushi.h"
  int yylex();
  void yyerror(const char* s);
%}

%union {
  char *s;
  int i;
  arglist_t alist;
  prog_t *exe;
  redirection_t redir;
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
%type<redir> out_redir any_redir in_redir out1_redir out2_redir inout_redir
%type<alist> args
%type<i> bg_mode
%type<exe> exe out_exe in_exe redir_exe pipe

%start cmdline

%%

cmdline: 
                       { /* an empty line is valid, too! Do nothing */ }
| redir_exe bg_mode    { spawn($1, NULL, $2); }
| in_exe pipe bg_mode  { spawn($1, $2, $3); }
| arg YY_SUSHI_SET arg { __not_implemented__(); } /* TODO */
| YY_SUSHI_JOBS        { __not_implemented__(); } /* TODO */
| YY_SUSHI_PWD         { __not_implemented__(); } /* TODO */
| YY_SUSHI_CD arg      { __not_implemented__(); } /* TODO */
| YY_SUSHI_HISTORY     { sushi_show_history(stdout); }
| YY_SUSHI_EXIT        { sushi_exit = 1; }

pipe: 
  YY_SUSHI_BAR out_exe  { __not_implemented__(); } /* TODO */
| YY_SUSHI_BAR exe pipe { __not_implemented__(); } /* TODO */

redir_exe: 
  exe           { $$ = $1; } 
| exe any_redir { $$ = $1; $$->redirection = $2; __not_implemented__(); }

in_exe:   
  exe           { $$ = $1; }
| exe in_redir  { $$ = $1; $$->redirection = $2; __not_implemented__(); }

out_exe:   
  exe           { $$ = $1; }
| exe out_redir { $$ = $1; $$->redirection = $2; __not_implemented__(); } 

inout_redir:    
  in_redir out_redir { $$.in = $1.in; $$.out1 = $2.out1; $$.out2 = $2.out2; }
| out_redir in_redir { $$.in = $2.in; $$.out1 = $1.out1; $$.out2 = $1.out2; }

out_redir:
  out1_redir { $$ = $1; } 
| out2_redir { $$ = $1; } 

any_redir:
  in_redir    { $$ = $1; }
| out_redir   { $$ = $1; } 
| inout_redir { $$ = $1; }

in_redir:   YY_SUSHI_LESS arg     { $$.in = $2; $$.out1 = $$.out2 = NULL; }
out1_redir: YY_SUSHI_MORE arg     { $$.out1 = $2; $$.in = $$.out2 = NULL; }
out2_redir: YY_SUSHI_MOREMORE arg { $$.out2 = $2; $$.in = $$.out1 = NULL; }

bg_mode: 
               { $$ = 0; }
| YY_SUSHI_AMP { $$ = 1; __not_implemented__(); }

exe: 
args {
  $$ = malloc(sizeof(prog_t));
  $$->args = $1;
  $$->redirection.in = $$->redirection.out1 = $$->redirection.out2 = NULL;
  $$->next = NULL; }

args:  
  arg      {
    $$.args = malloc(sizeof(char*)); // New argument array
    $$.args[0] = $1; // Its first element
    $$.size = 1; }
| args arg {
    $$ = $1;
    $$.size++; // Increase argument array size
    $$.args = realloc($$.args, sizeof(char*) * $$.size); // Add storage
    $$.args[$$.size - 1] = $2; } // Add the last element

arg: 
  YY_SUSHI_TOK { $$ = $1; }

%%

/* This section is empty */
