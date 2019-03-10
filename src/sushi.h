#ifndef SUSHI_H
#define SUSHI_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define SUSHI_MAX_INPUT 80 /* really modest :) */
#define SUSHI_HISTORY_LENGTH 32
#define SUSHI_DEFAULT_PROMPT "> "

char *sushi_read_line(FILE *in);
int sushi_read_config(char *fname);

void sushi_store(char *line);
void sushi_show_history();
char *sushi_unquote(char * s);
int sushi_parse_command(char *command);

extern int sushi_exit; // The global exit flag

// Support for the primitive parse tree
// An array of arguments, eventually NULL-terminated
typedef struct {
    int size;
    char **args;
} arglist_t;

// I/O redirection, as in "foobar < foo > bar"
typedef struct {
    char *in, *out1, *out2; // stdin, stdout-write, stdout-append
} redirection_t;

// The program to be executed
typedef struct prog {
    arglist_t args; // Arguments, including the program name
    redirection_t redirection; // Optional redirections
    struct prog *next; // The next program in the pipeline, if any; NULL otherwise
} prog_t;

// Start a new program
int sushi_spawn(prog_t *exe, prog_t *pipe, int bgmode);

// Report unimplemented functions
void __not_implemented__();

// Replace standard malloc and realloc: abort() if malloc/realloc fails
// The likelyhood of the event is low, but the consequences are grave
void *super_malloc(size_t size);
void *super_realloc(void *ptr, size_t size);
char *super_strdup (const char *s);

void sushi_assign(char *name, char *value);
char *sushi_safe_getenv(char *name);

#endif

