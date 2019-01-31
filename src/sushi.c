#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

int main(/*int argc, char *argv[]*/) {
    
    printf("%s", SUSHI_DEFAULT_PROMPT);
    char *fname = "foo.txt";
    FILE *filePointer = fopen(fname, "r");
    printf("Line 1: %s\n", sushi_read_line(filePointer));
    printf("Line 2: %s\n", sushi_read_line(filePointer));
    printf("Line 3: %s\n", sushi_read_line(filePointer));
    fclose(filePointer);
    
    //sushi_store(sushi_read_line(stdin));
    //sushi_show_history();
    
    
    /*//Testing for questions:
    char *name = "foo.txt";
    printf("\n");
    sushi_read_config(name);
    sushi_show_history();
     */
    
           
    return EXIT_SUCCESS;
}
/*command line:  gcc sushi_read.c sushi_history.c sushi.c -o program
*/
