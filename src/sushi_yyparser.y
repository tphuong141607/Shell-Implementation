%{
#include "sushi.h"
  int yylex();
  void yyerror(const char* s);

  // Find the first program on the command line
  static prog_t *last_exe(prog_t *exe) {
    while(exe->prev) exe = exe->prev;
  return exe;
}
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
| redir_exe bg_mode    { sushi_spawn($1, $2); }
| in_exe pipe bg_mode  { last_exe($2)->prev = $1; sushi_spawn($2, $3);}
| arg YY_SUSHI_SET arg { sushi_assign($1, $3); }
| YY_SUSHI_JOBS        { __not_implemented__(); } /* TODO */
| YY_SUSHI_PWD         { __not_implemented__(); } /* TODO */
| YY_SUSHI_CD arg      { __not_implemented__(); } /* TODO */
| YY_SUSHI_HISTORY     { sushi_show_history(stdout); }
| YY_SUSHI_EXIT        { sushi_exit = 1; }

pipe: 
  YY_SUSHI_BAR out_exe  { $$ = $2;}
| pipe YY_SUSHI_BAR out_exe { $$ = $3; $$->prev = $1;}

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
| YY_SUSHI_AMP { $$ = 1; }

exe: 
  args {
    $$ = super_malloc(sizeof(prog_t));
    $$->args = $1;
    $$->redirection.in = $$->redirection.out1 = $$->redirection.out2 = NULL;
    $$->prev = NULL; }

args:  
  arg      {
    $$.args = super_malloc(sizeof(char*)); // New argument array
    $$.args[0] = $1; // Its first element
    $$.size = 1; }
| args arg {
    $$ = $1;
    $$.size++; // Increase argument array size
    $$.args = super_realloc($$.args, sizeof(char*) * $$.size); // Add storage
    $$.args[$$.size - 1] = $2; } // Add the last element

arg: 
  YY_SUSHI_TOK { $$ = $1; }

%%

/* This section is empty */
