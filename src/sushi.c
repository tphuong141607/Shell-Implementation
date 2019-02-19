#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <stdbool.h>
#include <string.h>

// Function getenv() reads the value of a shell variable, such as $HOME.
int sushi_exit = 0;

int main() {
    /*
    char *file;
    char *fileName = "/sushi.conf";
    
    // read the commands from the file sushi.conf, located in the $HOME directory.
    file = malloc(strlen(getenv("HOME") + strlen(fileName) + 1));
    strcat(strcpy(file, getenv("HOME")), fileName);
    sushi_read_config(file);
    */
    
    while (sushi_exit == 0) {
        // display the prompt SUSHI_DEFAULT_PROMPT
        printf("%s", SUSHI_DEFAULT_PROMPT);
        
        char *returnValue = sushi_read_line(stdin);
        if (returnValue != NULL){
            // Call sushi_parse_command() if sushi_read_line() successfully called
            if (sushi_parse_command(returnValue) == 0) {
                // store the return value to history if no syntax error found.
                sushi_store(returnValue);
            };
        }
    }

    return EXIT_SUCCESS;
}

