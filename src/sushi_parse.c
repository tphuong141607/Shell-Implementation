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
    if (exe == NULL) {
        return;
    }
    
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
    
    free_memory(exe->prev);
}

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
    exit(0);
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

// Calculate the total size of the linkedList (total processes)
int linkedListSize(prog_t *head) {
    int count = 0;
    prog_t *currentNode = head;
    while (currentNode != NULL) {
        count++;
        currentNode = currentNode->prev;
    }
    return count;
}

// Calculate the sum of all values in an array
int calculateSum(int arr[], int n) {
    int total = 0;
    for (int i = 0; i < n; i++)
        total += arr[i];
    return total;
}

/*
 * You can use this function instead of yours if you want.
 */
int sushi_spawn_dz(prog_t *exe, int bgmode) {
  int pipe_length = 0, max_pipe = cmd_length(exe);
  pid_t pid[max_pipe];

  int old_stdout = STDOUT_FILENO;
  
  for(prog_t *prog = exe; prog; prog = prog->prev) {
    int pipefd[2] = {STDIN_FILENO, old_stdout};
    if (prog->prev && -1 == pipe(pipefd)) {
      perror("pipe");
      return 1;
    }
  
    switch(pid[pipe_length] = fork()) {
    case -1: // Error
      perror(prog->args.args[0]);
      return 1;
    case 0: // Child
      dup_me(pipefd[0], STDIN_FILENO);
      dup_me(old_stdout, STDOUT_FILENO);
      if(pipefd[1] != STDOUT_FILENO)
	close(pipefd[1]);
      start(prog);
      exit(1);
    default: // Parent
      if(pipefd[0] != STDIN_FILENO) close(pipefd[0]);
      if(old_stdout != STDOUT_FILENO) close(old_stdout);
      old_stdout = pipefd[1];
    }
    pipe_length++;
  }

  int status = 0;
  if (bgmode == 0) {
    for (int i = 0; i < pipe_length; i++)
      if(wait_and_setenv(pid[i]))
	status = 1;
  }

  if(old_stdout != STDOUT_FILENO) close(old_stdout);
  free_memory(exe);
  
  return status;
}
/*--------------------------------------------------------------------
 * End of "convenience" functions
 *--------------------------------------------------------------------*/

int sushi_spawn(prog_t *exe, int bgmode) {
    int totalProcess = linkedListSize(exe);
    int allProcessID[totalProcess];
    memset(allProcessID, 0, totalProcess*sizeof(int)); // initialize all elements in array to 0
    
    // Case 1: Only 1 Process - no pipes needed
    if (totalProcess == 1) {
        allProcessID[0] = fork();
        switch (allProcessID[0]) {
            case -1:
                perror("Fork() failed");
                return 1;
            case 0:
                start(exe);
                break;
            default:
                break;
        }
    }
    
    // Case 2: Multiple processes - pipes needed
    if (totalProcess > 1) {
        int pipe_fd[totalProcess][2]; // # of pipes = # of processes
        prog_t *currentNode = exe;
        int temp;
        
        for (int i = 0; i < totalProcess; i++) {
            pipe(pipe_fd[i]);
            allProcessID[i] = fork();
            
            switch (allProcessID[i]) {
                case -1: // Fork error
                    perror("Fork() failed");
                    return 1;
                    
                case 0: // in child process
                    // head process
                    if (currentNode->prev == NULL) {
                        dup_me(temp, STDOUT_FILENO);
                        close(pipe_fd[i][0]);
                        close(pipe_fd[i][1]);
                        start(currentNode);
                        break;
                    }
                    // Tail process
                    else if ((currentNode->prev != NULL) && (calculateSum(allProcessID, totalProcess)) == 0) {
                        close(pipe_fd[i][1]);
                        dup_me(pipe_fd[i][0], STDIN_FILENO); //send stdin to read end of pipe
                        start(currentNode);
                        break;
                    }
                    // middles process
                    else if (currentNode->prev != NULL) {
                        dup_me(temp, STDOUT_FILENO);
                        dup_me(pipe_fd[i][0], STDIN_FILENO);
                        close(pipe_fd[i][1]);
                        start(currentNode);
                        break;
                    }
                    
                default: // in parent process
                    // head process
                    if (currentNode->prev == NULL) {
                        close(temp);
                        close(pipe_fd[i][0]);
                        close(pipe_fd[i][1]);
                        break;
                    }
                    // Tail process
                    else if ((currentNode->prev != NULL) && (calculateSum(allProcessID, totalProcess)) == 0) {
                        temp = pipe_fd[i][1];
                        close(pipe_fd[i][0]);
                        break;
                    }
                    // middles process
                    else if (currentNode->prev != NULL) {
                        close(temp);
                        temp = pipe_fd[i][1];
                        close(pipe_fd[i][0]);
                        break;
                    }
            }
            currentNode = currentNode->prev;
        }
    }
    
    // In parent
    free_memory(exe);
    if(bgmode == 0) {
        for (int i = 0; i < totalProcess; i++){
            wait_and_setenv(allProcessID[i]);
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

/*
 * New skeleton functions
 */
void sushi_display_wd() {
}

void sushi_change_wd(char *new_wd) {
}
