#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <stdbool.h>
#include <string.h>

int sushi_exit = 0;

/* Sets up a signal handler that intercepts SIGINT and
 displays message “Type exit to exit the shell” on stderr. */
static void refuse_to_die(int sig) {
    if (sig == SIGINT) {
        printf("Type exit to exit the shell");
    }
}

static void prevent_interruption() {
    struct sigaction sVal;
    memset(&sVal, 0, sizeof(sVal));
    sVal.sa_handler = refuse_to_die;
    sigaction(SIGINT, &sVal, NULL);
}

/* Main function */
int main() {
    char *file;
    char *fileName = "/sushi.conf";
    
    // read the commands from the file sushi.conf, located in the $HOME directory.
    file = super_malloc(strlen(getenv("HOME")) + strlen(fileName) + 1);
    strcat(strcpy(file, getenv("HOME")), fileName);
    sushi_read_config(file);
    prevent_interruption();
    
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
