#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

static char *history[SUSHI_HISTORY_LENGTH] = {NULL};

void sushi_store(char *line) {
    int stackPointer = SUSHI_HISTORY_LENGTH-1;
    
    if (stackPointer == 0) {
        free(history[SUSHI_HISTORY_LENGTH-1]);
    }
    
    /* All existing lines are shifted to the next position */
    for (int i = SUSHI_HISTORY_LENGTH; i > 0; i--) {
        history[i] = history[i-1];
    }
        /* The newly inserted line becomes the 1st element of the history */
        history[0] = line;
    
    printf("Pos 0: %s\n", history[0]);
    printf("Pos 1: %s\n", history[1]);
    printf("Pos 2: %s\n", history[2]);
    printf("Pos 2: %s\n", history[3]);
    printf("Pos 2: %s\n", history[4]);
    
    
}

void sushi_show_history() {
}

