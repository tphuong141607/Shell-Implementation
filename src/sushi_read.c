#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "sushi.h"

char *sushi_read_line(FILE *in) {
    char lineBuffer[SUSHI_MAX_INPUT];
    int bufferLen = 0;
    
    /* Read a line to lineBuffer*/
    if (fgets(lineBuffer, SUSHI_MAX_INPUT, in) == NULL) {
        return NULL;
    };
    
    /* This length includes newline but not terminator */
    bufferLen = strlen(lineBuffer);
    
    /* Remove newline character */
    if(lineBuffer[bufferLen - 1] == '\n') {
        lineBuffer[bufferLen - 1] = '\0';
    }
    
    /* This new length does not include newline nor terminator */
    bufferLen = strlen(lineBuffer);
    
    /* Return NULL if the line is empty or consists of only whitespace*/
    int noSpaceCount = 0;
    for(int i = 0; i < bufferLen; i++) {
        if (!isspace(lineBuffer[i])) {
            noSpaceCount++;
        }
    }
    
    char *lineAddress = NULL;
    if (!((noSpaceCount == 0) || (lineBuffer[0] == 0))) {
        lineAddress = super_malloc((bufferLen + 1) * sizeof(*lineBuffer));
        strcpy(lineAddress, lineBuffer);
    }
    
    /* Discarding the rest of the line if line is too long...
     While the file is still readable, call fgets() */
    if (bufferLen == SUSHI_MAX_INPUT - 1) {
        fprintf(stderr, "Warning: Line too long, truncated\n");
        while (fgets(lineBuffer, SUSHI_MAX_INPUT, in) == NULL) {}
    }
    
    /* Returning the address of the line */
    return lineAddress;
}


/* Read all lines from the file fname */
int sushi_read_config(char *fname, int ok_if_missing) {
    FILE *filePointer;
    
    if (ok_if_missing == 0){ //PART V
        exit(1);
        
    } else {
        /* Check if file exists */
        if(access(fname, F_OK) != -1 ) {
            filePointer = fopen(fname, "r");
            
            /* Check if file can be opened */
            if (filePointer == NULL) {
                perror(fname);
            } else {
                /* Read and store each line using sushi_read_line and sushi_store */
                do {
                    char *line = sushi_read_line(filePointer);
                    if (line != NULL) {
                        if (sushi_parse_command(line) == 0) {
                            sushi_store(line);
                        }
                    } else {
                        perror(fname);
                    }
                } while ((!feof(filePointer)));
                
                /* Close the file */
                fclose(filePointer);
                return 0;
            }
        } else {
            perror(fname);
        }
    }
    return 1;
}





