#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sushi.h"

static char *history[SUSHI_HISTORY_LENGTH] = {NULL};

void sushi_store(char *line) {

    /* If line is NULL, do nothing */
    if (line == NULL) {
        return;
    }

    /* If the history is full (meaning the last element in history is not NULL),
     free the memory allocated for that last element*/
    if ( history[SUSHI_HISTORY_LENGTH - 1] != NULL) {
        free(history[SUSHI_HISTORY_LENGTH - 1]);
    }
    
    /* All existing lines are shifted to the next position */
    for (int i = SUSHI_HISTORY_LENGTH - 1; i >= 0; i--) {
        *(history + i) = *(history + i-1);
    }
    
    /* The newly inserted line becomes the 1st element of the history */
    *history = line;
    printf("Line: %s\n", line);
    printf("History: %s\n", *history);
    
}

void sushi_show_history() {
    printf("\n");
    for (int i = 0; i < SUSHI_HISTORY_LENGTH; i++) {
        if (history[i] != NULL) {
            printf("%0.5d  %s\n", (i+1), history[i]);
        }
    }
}

