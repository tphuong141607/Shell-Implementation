#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

int main(/*int argc, char *argv[]*/) {
    char *source = "sushi.conf";
    sushi_read_config(source);
    
    printf("%s", SUSHI_DEFAULT_PROMPT);
    sushi_store(sushi_read_line(stdin));

    sushi_show_history();
    
    return EXIT_SUCCESS;
}
/*command line:  gcc sushi_read.c sushi_history.c sushi.c -o program
*/
