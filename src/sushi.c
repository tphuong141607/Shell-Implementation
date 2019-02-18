#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <stdbool.h>
#include <string.h>

// Function getenv() reads the value of a shell variable, such as $HOME.

int main() {
    char *file;
    char *fileName = "/sushi.conf";
    
    // read the commands from the file sushi.conf, located in the $HOME directory.
    file = malloc(strlen(getenv("HOME") + strlen(fileName) + 1));
    strcat(strcpy(file, getenv("HOME")), fileName);
    sushi_read_config(file);
    
    // display the prompt SUSHI_DEFAULT_PROMPT
    printf("%s", SUSHI_DEFAULT_PROMPT);
    
    // read a command from stdin using sushi_read_line
    // and store the line into the history
    sushi_store(sushi_read_line(stdin));
    
    //Display the content of the history
    sushi_show_history();
    return EXIT_SUCCESS;
}

