#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <stdbool.h>

int main() {
    if (sushi_read_config("sushi.conf") == 1) {
        return EXIT_FAILURE;
    }
    printf("%s", SUSHI_DEFAULT_PROMPT);
    sushi_store(sushi_read_line(stdin));
    sushi_show_history();
    return EXIT_SUCCESS;
}

