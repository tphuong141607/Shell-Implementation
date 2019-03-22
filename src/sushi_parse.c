#include <string.h>
#include "sushi.h"
#include "sushi_yyparser.tab.h"

//Using python works, but I have to double type 1 character... Why?

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
    
    // free each non-NULL argument in the array exe->args.args
    for(int i = 0; i < exe->args.size; i++ ) {
        if (exe->args.args[i] != NULL) {
            free(exe->args.args[i]);
        }
    }
    
    // Free the array itself
    free(exe->args.args);
    
    // Free each non-NULL exe->redirection
    if (exe->redirection.in != NULL) {
        free(exe->redirection.in);
    }
    if (exe->redirection.out1 != NULL) {
        free(exe->redirection.out1);
    }
    if (exe->redirection.out2 != NULL) {
        free(exe->redirection.out2);
    }
    
    // Free exe itself
    free(exe);
    
    // For next assignment
    pipe;
    /* Abort if try to free(pipe);
    if (pipe != NULL) {
        free(pipe);
    }
    */
}

// Skeleton
void sushi_assign(char *name, char *value) {
    /* If name does exist in the environment, then its value
     * is changed to value if overwrite is nonzero;
     * The setenv() function returns zero on success,
     * or -1 on error, with errno set to indicate the cause of the error.
     */
    int returnValue = setenv(name, value, 1);
    free(value);
    free(name);
}

// Skeleton
char *sushi_safe_getenv(char *name) {
    /*The getenv() function returns a pointer to the
     * value in the environment, or NULL if there is no match. */
    char *returnPointer = getenv(name);
    if (returnPointer == NULL) {
        return "";
    }
    return returnPointer;
}

int sushi_spawn(prog_t *exe, prog_t *pipe, int bgmode) {
    // Fork a child process
    pid_t  pid, endID;
    pid = fork();
    
    switch (pid) {
        // Fork() fail (2 cases: too many processes, memory run-out)
        case -1:
            perror("Fork() failed");
            return 1;
            
        // In the child process
        case 0:
            exe->args.args = super_realloc(exe->args.args, ((exe->args.size + 1) * sizeof(prog_t)));
            exe->args.args[exe->args.size] = NULL;
            
            if (execvp(exe->args.args[0], exe->args.args) < 0) {
                perror(exe->args.args[0]);
                exit(0);
            }
        
        // In the parent process
        int statusPtr;
        default:
            if (bgmode == 1) {
                free_memory(exe, pipe);
                
            } else if(bgmode == 0) {
                free_memory(exe, pipe);
                endID = waitpid(pid, &statusPtr, 0);
                
                if (endID != -1) { // waitpid works
                    char status[4];
                    sprintf(status, "%d", statusPtr); // sprintf: write formatted data to string
                    setenv("_", status, 1);
                }
            }
    }
    return 0;
}

/*The wrapper functions shall call the corresponding library functions.
 If the library function does not fail, the wrapper will return
 the pointer to the newly allocated memory. */
char *super_strdup (const char *s) {
    char *newPointerStrdup = strdup(s);
    if (newPointerStrdup == NULL) {
        abort();
    }
    return newPointerStrdup;
}

// provide * sizeof(datatype) when called
void *super_malloc(size_t size) {
    void *newPointerMalloc = malloc(size);
    if (newPointerMalloc == NULL) {
        abort();
    }
    return newPointerMalloc;
}

// provide * sizeof(datatype) when called
void *super_realloc(void *ptr, size_t size) {
    void *newPointerRealloc = realloc(ptr, size);
    if (newPointerRealloc == NULL) {
        abort();
    }
    return newPointerRealloc;
}
