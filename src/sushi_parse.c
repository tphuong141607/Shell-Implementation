#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
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
void free_memory(prog_t *exe) {
    
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
    
}

// Skeleton
void sushi_assign(char *name, char *value) {
    /* If name does exist in the environment, then its value
     * is changed to value if overwrite is nonzero;
     * The setenv() function returns zero on success,
     * or -1 on error, with errno set to indicate the cause of the error.
     */
    if(setenv(name, value, 1) != 0) {
        perror("setenv");
    }
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

/*------------------------------------------------------------------
 * You can use these "convenience" functions as building blocks for
 * HW5 instead of your code, if you want. You may change them, too.
 *------------------------------------------------------------------*/

// Find the number of programs on the command line
static size_t cmd_length(prog_t *exe) {
  int count = 0;
  while(exe->prev) {
    exe = exe->prev;
    count++;
  }
  return count;
}

// Wait for the process pid to terminate; once it does, set the
// environmental variable "_" to the exit code of the process.
static int wait_and_setenv(pid_t pid) {
  int status;
  if (-1 == waitpid(pid, &status, 0)) {
    perror("waitpid");
    status = 1; // Something bad happened
  }
  char retval[16]; // Just to be safe
  sprintf(retval, "%d", status);
  if(-1 == setenv("_", retval, 1)) {
    perror("_");
    return 1;
  } else
    return 0;
}

// Execute the program defined in "exe"
static void start(prog_t *exe) {
  arglist_t args = exe->args;
  args.args = realloc(args.args, sizeof(char*) * (args.size + 1));
  args.args[args.size] = (char*)NULL;
  execvp(args.args[0], args.args);
  perror(args.args[0]);
}

// "Rename" fule descriptor "old" to "new," if necessary. After the
// execution of this function a program that "believes" that it uses
// the "old" descriptor (e.g., stdout #1 for output) will be actually
// using the "new" descriprot (e.g., an outgoing pipe).  This
// functions terminates the process of error and should not be used in
// the parent, only in a child.
static void dup_me (int new, int old) {
  if (new != old && -1 == dup2(new, old)) {
    perror("dup2");
    exit(1);
  }
}

/*--------------------------------------------------------------------
 * End of "convenience" functions
 *--------------------------------------------------------------------*/

int sushi_spawn(prog_t *exe, int bgmode) {
    // Fork a child process
    pid_t  pid, endID;
    pid = fork();
    
    switch (pid) {
        // Fork() fail (2 cases: too many processes, memory run-out)
        case -1:
            perror("Fork() failed");
            return 1;
            
        // In the child process
        /* execvp() Definition and How to Use it
         • execvp(): The first argument is the file you wish to execute, and the second argument is an
         array of null-terminated strings that represent the appropriate arguments.
         • Example:
                char *cmd = "ls";
                char *argv[3]; argv[0] = "ls"; argv[1] = "-la"; argv[2] = NULL;
                execvp(cmd, argv); //This will run "ls -la" as if it were a command
        */
        case 0:
            exe->args.args = super_realloc(exe->args.args, ((exe->args.size + 1) * sizeof(prog_t)));
            exe->args.args[exe->args.size] = NULL;
            
            if (execvp(exe->args.args[0], exe->args.args) < 0) {
                perror(exe->args.args[0]);
                exit(0);
            }
	    break;
        
        // In the parent process
        /* bgmode definition:
         If bgmode is 1, the function shall only free_memory(exe).
         If bgmode is 0, the parent shall:
            1. Free the memory
            2. Wait for the child to terminate with waitpid()
            3. Collect the value returned by the child and assign it to the
            environmental variable _ (underscore). Remember that setenv()
            expects the value to be a string.
         */
        int statusPtr;
        default:
            free_memory(exe);
            if(bgmode == 0) {
                endID = waitpid(pid, &statusPtr, 0);
                
                if (endID != -1) { // waitpid works
                    char status[4];
                    sprintf(status, "%d", statusPtr); // sprintf: write formatted data to string
                    setenv("_", status, 1);
                } else {
                    perror("endID");
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



