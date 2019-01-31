#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "sushi.h"


char *sushi_read_line(FILE *in) {
    char lineBuffer[SUSHI_MAX_INPUT];
    int bufferLen = 0; // length of the string
    int countSpace;
    char *lineAddress;

    /* read a line to lineBuffer*/
    fgets(lineBuffer, SUSHI_MAX_INPUT, in);
    bufferLen = strlen(lineBuffer); //not including the terminator, include new line
    printf("bufferLen: %d\n", bufferLen );
   
    // lineBuffer[bufferLen] = 0;
    
    /*
    printf("%s\n", lineBuffer);
    printf("bufferLen: %d\n", bufferLen );
    printf("bufferSize: %d\n", (int)sizeof(lineBuffer));
    */
    

    /* GOOD if the line is longer than the allowed size, print warning */
    if (bufferLen == SUSHI_MAX_INPUT - 1) {
        fprintf(stderr, "Warning: Line too long, truncated\n");
    }
        
    /* GOOD return NULL if the line consists of only whitespace */
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
    
    /* Discarding the rest of the line if line is too long... */
    while(lineBuffer[bufferLen] != 0) {
        printf("char: %c\n", lineBuffer[bufferLen-2]);
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
        
        /* If line is NULL, do nothing  CHECKING */
        if (line == NULL) {
            return 0;
        }
        
        sushi_store(line); //split - check NULL
        
    }
    /*
    printf("%s\n", sushi_read_line(filePointer));
    printf("%s\n", sushi_read_line(filePointer));
    printf("%s\n", sushi_read_line(filePointer));
    printf("%s\n", sushi_read_line(filePointer));
    printf("%s\n", sushi_read_line(filePointer));
    printf("%s\n", sushi_read_line(filePointer));
    printf("%s", sushi_read_line(filePointer));
    printf("%s", sushi_read_line(filePointer));
    */
    fclose(filePointer);
    
  return 0;
}






