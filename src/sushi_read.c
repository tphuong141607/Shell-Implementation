#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "sushi.h"


char *sushi_read_line(FILE *in) {
    char ch;
    char *lineBuffer; // Store the address of the 1st character of a line
    int bufferLen = 0; // Size of the "array of characters"
    int countChar = 0; // used to check if total characters in the file > SUSHI_MAX_INPUT
    
    if (in != NULL) {
        /* Allocate memory for the buffer */
        lineBuffer = (char *)malloc(SUSHI_MAX_INPUT * sizeof(*lineBuffer));

        /* return NULL if fail to allocate memory */
        if (lineBuffer == NULL) {
            free(lineBuffer);
            perror("Error: failed to allocate memory for line buffer.\n");
            return NULL;
        }

        /* Store input into the newly allocated buffer*/
        /* If the input is longer than the SUSHI_MAX_INPUT, discards the rest */
        ch = fgetc(in);
        while (ch != '\n' && ch != EOF) {
            countChar++;
            if (bufferLen < SUSHI_MAX_INPUT) {
                lineBuffer[bufferLen++] = ch;
            }
            ch = fgetc(in);
        }
        if (countChar > SUSHI_MAX_INPUT) {
            fprintf(stderr, "Warning: Line too long, truncated\n");
        }
        
        /* return NULL if the line consists of only whitespace */
        int countSpace = 0;
        for(int i = 0; i < bufferLen; i++) {
            if (isspace(lineBuffer[i])) {
                countSpace++;
            }
        }
        if (countSpace == bufferLen && bufferLen != 0) {
            free (lineBuffer);
            return NULL;
        }
        
        /* return NULL if the line is empty */
        if (bufferLen == 0 && lineBuffer[bufferLen] == '\0') {
            free (lineBuffer);
            return NULL;
        }
        
        return lineBuffer;

    } else {
        perror ("Error: file pointer is null.\n");
        return NULL;
    }
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
        sushi_store(sushi_read_line(filePointer));
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






