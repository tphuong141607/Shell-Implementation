#ifndef SUSHI_H
#define SUSHI_H

#include <stdio.h>

#define SUSHI_MAX_INPUT 80 /* really modest :) */
#define SUSHI_HISTORY_LENGTH 32
#define SUSHI_DEFAULT_PROMPT "> "

char *sushi_read_line(FILE *in);
int sushi_read_config(char *fname);

void sushi_store(char *line);
void sushi_show_history();
  
#endif
