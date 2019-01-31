#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "sushi.h"


char *sushi_read_line(FILE *in) {
    char lineBuffer[SUSHI_MAX_INPUT];
    int bufferLen = 0;
    int countSpace;
    char *lineAddress;

    /* read a line to lineBuffer*/
    fgets(lineBuffer, SUSHI_MAX_INPUT, in);
    bufferLen = strlen(lineBuffer); // include newline but not terminator
    
    /* remove newline character */
    if(lineBuffer[bufferLen - 1] == '\n') {
        lineBuffer[bufferLen - 1] = 0;
    }
    bufferLen = strlen(lineBuffer); //not include either terminator nor newline
    lineBuffer[bufferLen] = 0; // assignment terminator to the end of the string
    
    /* return NULL if the line consists of only whitespace */
    countSpace = 0;
    for(int i = 0; i < bufferLen; i++) {
        if (isspace(lineBuffer[i])) {
            countSpace++;
        }
    }
    if (countSpace == bufferLen && bufferLen != 0) {
        return NULL;
    }
        
    /* return NULL if the line is empty */
    if (bufferLen == 0 && lineBuffer[bufferLen] == '\0') {
        return NULL;
    }
    
    /* allocate memory to store the finalized line */
    lineAddress = malloc((size_t)(bufferLen + 1) * sizeof(*lineBuffer));
    strcpy(lineAddress, lineBuffer);
    
    /* discarding the rest of the line if line is too long...
     While the file is still readable, call fgets() */
    /* if the line is longer than the allowed size, print warning */
    if (bufferLen == SUSHI_MAX_INPUT - 1) {
        fprintf(stderr, "Warning: Line too long, truncated\n");
        while (fgets(lineBuffer, SUSHI_MAX_INPUT, in) == NULL) {
            printf("here");
        }
    }
    
    /* returning the address of the line */
    return lineAddress;
}



int sushi_read_config(char *fname) {
    FILE *filePointer;
    
    /* check if file exists */
    if(access( fname, F_OK ) != -1 ) {
        filePointer = fopen(fname, "r");
    } else {
        return 1;
    }

    /* check if file can be opened */
    if (filePointer == NULL){
        perror("Error: could not open file\n");
        return 1;
    }
    
    /* Read and store each line using sushi_read_line and sushi_store */
    while ((!feof(filePointer))) {
        char *line = sushi_read_line(filePointer);

        if (line != NULL) {
            sushi_store(line);
        }
    }
    fclose(filePointer);
  return 0;
}






