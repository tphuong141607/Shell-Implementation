#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"


void sushi_store(char *line) {
    
    history = (char *)malloc(sizeof(*history) * SUSHI_HISTORY_LENGTH);
    
    if (history == NULL) {
        free(history);
        perror("Error: failed to allocate memory for shell history.");
    }
    
    for (int i = SUSHI_HISTORY_LENGTH; i > 0; i--) {
        history[i] = history[i-1];
    }
    history[0] = &line;
    
    printf("The word: %s\n", *shellHistory[0]);
    
    /*for (int i = 0; i < SUSHI_HISTORY_LENGTH; i++) {
       printf("The word: %s\n", shellHistory[i]);
    }
     */
    
}

void sushi_show_history() {
}

