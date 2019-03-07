#include <string.h>
#include "sushi.h"
#include "sushi_yyparser.tab.h"

char *sushi_unquote(char *s) {
    int sINDX = 0;
    int len = strlen(s);
    
    while (s[sINDX] != 0) {
        // If whitespace characters, enter this part of the code.
        if ((s[sINDX] == '\\') && (s[sINDX+1] != 0)) {
            switch(s[sINDX+1]){
                case 'a': s[sINDX] = '\a'; break;
                case 'b': s[sINDX] = '\b'; break;
                case 'f': s[sINDX] = '\f'; break;
                case 'n': s[sINDX] = '\n'; break;
                case 'r': s[sINDX] = '\r'; break;
                case 't': s[sINDX] = '\t'; break;
                case 'v': s[sINDX] = '\v'; break;
                case '\\': s[sINDX] = '\\'; break;
                case '\'': s[sINDX] = '\''; break;
                case '"': s[sINDX] = '\"'; break;
                case '?': s[sINDX] = '\?'; break;
            }
            /* If there is nothing after the whitespace characters,
               simply assign NULL terminator to s[sINdx + 1]
               Example: '\' and 'n' --> '\n' and '\0' */
            if (s[sINDX + 2] == '\0') {
                s[sINDX + 1] = '\0';
                
            } else {
                /* If we still have some characters after the whitespace characters
                   shift all characters starting from sINDX+1 until the end of the string
                   Example: '\' and 'n' and 'a' --> '\n' and 'a' */
                for (int i = sINDX+1; i < len; i++) {
                    s[i] = s[i + 1];
                }
            }
        }
        // If not whitespace characters, simply increment
        sINDX++;
    }
    return s;
}

// Do not modify these functions
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
}

void __not_implemented__() {  
  fputs("This operation is not implemented yet\n", stderr);
}

// Function skeletons for HW3
void free_memory(prog_t *exe, prog_t *pipe) {
  // Temporary solve the unused parameter errors
    exe;
    pipe;
  // TODO - but not this time
}

int spawn(prog_t *exe, prog_t *pipe, int bgmode) {
    // Temporary solve the unused parameter errors
    bgmode;
    
    pid_t  pid;
    int returnStatus;
    
    // Fork a child process
    pid = fork();
    
    /* Add another element to the array of arguments with realloc() and
     set that element to NULL.*/
    
    //realloc(exe, (exe->args.size + 1) * sizeof(prog_t));
    exe = super_realloc(exe, (exe->args.size + 1));
    exe->args.args[exe->args.size] = NULL;
    
    /* Start new program, as defined by exe->args.args[0]. If execvp()
     fails, the function shall exit(0). Remember, only the child
     terminates, the parent keeps running!*/
    
    if (pid == 0) { //child process
        // execvp(const char *file, char *const argv[]);
        if (execvp(exe->args.args[0], exe->args.args) < 0) {
            exit(0);
        }
    } else { //parent process
        free_memory(exe, pipe);
        printf("free_memory is called");
        
        wait(&returnStatus); // Wait for the child process to exit.
        if (returnStatus == -1) {// The child process execution failed.
            perror("execv");
            return 1;
        }
    }
  return 0;
}

/*The wrapper functions shall call the corresponding library functions.
 If the library function does not fail, the wrapper will return
 the pointer to the newly allocated memory. */

// Strdup() If an error occurs, a null pointer is returned and errno may be set.
char *super_strdup (const char *s) {
    char *newPointer = strdup(s);
    if (newPointer == NULL) {
        abort();
    }
    return newPointer;
}

void *super_malloc(size_t size) {
    char *newPointer = malloc(size * sizeof(prog_t));
    if (newPointer == NULL) {
        abort();
    }
    return newPointer;
}

void *super_realloc(void *ptr, size_t size) {
    char *newPointer = realloc(ptr, size * sizeof(prog_t));
    if (newPointer == NULL) {
        abort();
    }
    return newPointer;
}
