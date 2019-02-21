#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <stdbool.h>
#include <string.h>

int sushi_exit = 0;

int main() {
    
    char *file;
    char *fileName = "/sushi.conf";
    
    // read the commands from the file sushi.conf, located in the $HOME directory.
    file = malloc(strlen(getenv("HOME")) + strlen(fileName) + 1);
    strcat(strcpy(file, getenv("HOME")), fileName);
    sushi_read_config(file);
    
    while (sushi_exit == 0) {
        // display the prompt SUSHI_DEFAULT_PROMPT
        printf("%s", SUSHI_DEFAULT_PROMPT);
        
        char *commandLine = sushi_read_line(stdin);
        if (commandLine != NULL){
            // Call sushi_parse_command() if sushi_read_line() is successfully called
            if (sushi_parse_command(commandLine) == 0) {
                // store the returnValue to history if no syntax error found.
                sushi_store(commandLine);
            };
        }
    }
    return EXIT_SUCCESS;
}

