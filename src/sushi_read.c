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
    // DZ: Must check the returned value
    fgets(lineBuffer, SUSHI_MAX_INPUT, in);
    
    /* This length includes newline but not terminator */
    bufferLen = strlen(lineBuffer);
    
    /* Remove newline character */
    if(lineBuffer[bufferLen - 1] == '\n') {
        lineBuffer[bufferLen - 1] = '\0';
    }

    // DZ: Why would you want to do this? This is wrong.
    /* Remove non-ASCII character */
    int indx = 0;
    while (indx < bufferLen) {
        if(!isascii(lineBuffer[indx])) {
            lineBuffer[0] = '\0';
            break;
        }
        indx++;
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
        lineAddress = (char *)malloc((size_t)(bufferLen + 1) * sizeof(*lineBuffer));
        if (lineAddress == NULL) {
            perror("Error: ");
            return NULL;
        }
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
int sushi_read_config(char *fname) {
    FILE *filePointer;
    
    /* Check if file exists */
    if(access( fname, F_OK ) != -1 ) {
        filePointer = fopen(fname, "r");
    } else {
        return 1;
    }

    /* Check if file can be opened */
    if (filePointer == NULL){
      // DZ: perror(fname)
        perror("Error: ");
        return 1;
    }
    /* Read and store each line using sushi_read_line and sushi_store */
    do {
        char *line = sushi_read_line(filePointer);
        if (line != NULL) {
            sushi_store(line);
        }
    } while ((!feof(filePointer)));
  
    /* Close the file */
    fclose(filePointer);
  return 0;
}






